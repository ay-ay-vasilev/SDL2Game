#pragma once

#include <SDL_stdinc.h>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

namespace ecs
{
	class Component;
	class DrawableComponent;
	class Entity;
	class System;
	class Manager;

	using ComponentID = std::size_t;

	inline ComponentID getNewComponentTypeID()
	{
		static ComponentID lastID = 0u;
		return lastID++;
	}

	template <typename T> inline ComponentID getComponentTypeID() noexcept
	{
		static ComponentID typeID = getNewComponentTypeID();
		return typeID;
	}

	constexpr std::size_t inline maxComponents = 32;

	using ComponentBitSet = std::bitset<maxComponents>;
	using ComponentArray = std::array<std::shared_ptr<Component>, maxComponents>;

	class Component
	{
	public:
		virtual ~Component() {}
		Entity* entity = nullptr;
		virtual void init() {}
		virtual void update(double delta) {}
	};

	class DrawableComponent : public Component
	{
	public:
		virtual ~DrawableComponent() {}
		virtual void draw() {}
		void setRenderOrder(int renderOrder) { this->renderOrder = renderOrder; }
		const int getRenderOrder() const { return renderOrder; }
	private:
		int renderOrder = 0;
	};

	class Entity
	{
	public:
		Entity(Manager& manager, int nextID) : manager(manager), id(nextID), componentArray{} {}
		void update(double delta)
		{
			for (auto component : components) component->update(delta);
		}
		void draw()
		{
			for (auto& drawable : drawableComponents) drawable->draw();
		}
		bool isActive() { return active; }
		void destroy() { active = false; }

		template <typename T> bool hasComponent() const
		{
			return componentBitSet[getComponentTypeID<T>()];
		}

		template <typename... ComponentTypes> bool hasComponents() const
		{
			return (hasComponent<ComponentTypes>() && ...);
		}

		template <typename T, typename... TArgs>
		std::shared_ptr<T> addComponent(TArgs&&... mArgs)
		{
			std::shared_ptr<T> component = std::make_shared<T>(std::forward<TArgs>(mArgs)...);
			component->entity = this;
			components.emplace_back(component);
			componentArray[getComponentTypeID<T>()] = component;
			componentBitSet[getComponentTypeID<T>()] = true;
			component->init();

			auto drawable = std::dynamic_pointer_cast<DrawableComponent>(component);
			if (drawable)
			{
				drawableComponents.emplace_back(drawable);
				std::sort(drawableComponents.begin(), drawableComponents.end(),
					[](const auto& lhs, const auto& rhs)
					{
						return lhs->getRenderOrder() < rhs->getRenderOrder();
					});
			}
			return component;
		}

		template <typename T>
		void removeComponent()
		{
			if (hasComponent<T>())
			{
				componentBitSet[getComponentTypeID<T>()] = false;
				componentArray[getComponentTypeID<T>()] = nullptr;
				auto drawableIt = std::find_if(drawableComponents.begin(), drawableComponents.end(),
					[](const std::shared_ptr<DrawableComponent>& c)
					{
						return std::dynamic_pointer_cast<T>(c) != nullptr;
					});
				if (drawableIt != drawableComponents.end())
				{
					drawableComponents.erase(drawableIt);
					std::sort(drawableComponents.begin(), drawableComponents.end(),
						[](const auto& lhs, const auto& rhs)
						{
							return lhs->getRenderOrder() < rhs->getRenderOrder();
						});
				}

				auto it = std::find_if(components.begin(), components.end(),
					[](const std::shared_ptr<Component>& c)
					{
						return std::dynamic_pointer_cast<T>(c) != nullptr;
					});
				if (it != components.end())
				{
					components.erase(it);
				}
			}
		}

		template<typename T>
		std::shared_ptr<T> getComponent() const
		{
			std::shared_ptr<Component> component = componentArray[getComponentTypeID<T>()];
			if (component) {
				return std::dynamic_pointer_cast<T>(component);
			}
			return nullptr;
		}

		const Uint32 getID() const { return id; }

	private:
		Uint32 id;
		Manager& manager;
		bool active = true;
		std::vector<std::shared_ptr<Component>> components;
		std::vector<std::shared_ptr<DrawableComponent>> drawableComponents;

		ComponentArray componentArray;
		ComponentBitSet componentBitSet;
	};

	class System
	{
	public:
		System(Manager& manager) : manager(manager) {}
		virtual ~System() {}

		// Disallow copy
		System(const System&) = delete;
		System& operator=(const System&) = delete;

		// Disallow move
		System(System&&) = delete;
		System& operator=(System&&) = delete;

		virtual void init() {}
		virtual void update(double delta) {}
		virtual void draw() {}
	protected:
		Manager& manager;
	};

	class Manager
	{
	public:
		Manager() {}

		void update(double delta)
		{
			for (const auto& entity : entities) entity->update(delta);
			for (const auto& system : systems) system->update(delta);
		}

		void draw()
		{
			for (const auto& system : systems) system->draw();
		}

		void refresh()
		{
			entities.erase(std::remove_if(std::begin(entities), std::end(entities),
				[](const std::unique_ptr<Entity>& mEntity)
				{
					return !mEntity->isActive();
				}),
				std::end(entities));
		}

		Entity& addEntity()
		{
			auto entity = std::make_unique<Entity>(*this, nextID++);
			entities.emplace_back(std::move(entity));
			return *entities.back();
		}

		template <typename T>
		std::shared_ptr<T> addSystem()
		{
			std::shared_ptr<T> system = std::make_shared<T>(*this);
			systems.emplace_back(system);
			system->init();
			return system;
		}

		template <typename T>
		std::vector<Entity*> getEntitiesWithComponent()
		{
			std::vector<Entity*> entitiesWithComponent;
			for (const auto& entity : entities) {
				if (entity->hasComponent<T>()) {
					entitiesWithComponent.push_back(entity.get());
				}
			}
			return entitiesWithComponent;
		}

		template <typename... ComponentTypes>
		std::vector<Entity*> getEntitiesWithComponents()
		{
			std::vector<Entity*> entitiesWithComponents;
			for (const auto& entity : entities) {
				if (entity->hasComponents<ComponentTypes...>()) {
					entitiesWithComponents.push_back(entity.get());
				}
			}
			return entitiesWithComponents;
		}

		float getScale() { return scale; }
		void setScale(const float newScale) { scale = newScale; }

	private:
		std::vector<std::unique_ptr<Entity>> entities;
		std::vector<std::shared_ptr<System>> systems;
		float scale = 1.f;
		int nextID = 0;
	};
}