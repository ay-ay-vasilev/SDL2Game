#pragma once

#include "Constants.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class DrawableComponent;
class Entity;
class System;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

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

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<std::shared_ptr<Component>, maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;

class Component
{
public:
	Entity* entity = nullptr;
	virtual void init() {}
	virtual void update() {}

	virtual ~Component() {}
};

class DrawableComponent : public Component
{
public:
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
	void update()
	{
		for (auto component : components) component->update();
	}
	void draw() 
	{
		for (auto& drawable : drawableComponents) drawable->draw();
	}
	bool isActive() { return active; }
	void destroy() { active = false; }

	bool hasGroup(Group group)
	{
		return groupBitSet[group];
	}

	void addGroup(Group group);
	void delGroup(Group group)
	{
		groupBitSet[group] = false;
	}

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

		componentArray[getComponentTypeID<T>()] = component;
		componentBitSet[getComponentTypeID<T>()] = true;

		component->init();
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

	int getID() const { return id; }

private:
	int id;
	Manager& manager;
	bool active = true;
	std::vector<std::shared_ptr<Component>> components;
	std::vector<std::shared_ptr<DrawableComponent>> drawableComponents;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;
};

class System
{
public:
	System(Manager& manager) : manager(manager) {}
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~System() {}

protected:
	Manager& manager;
};

class Manager
{
public:
	Manager(std::shared_ptr<Constants> constants) : constants(constants) {}

	void update()
	{
		for (const auto& entity : entities) entity->update();
		for (const auto& system : systems) system->update();
	}
	
	void draw()
	{
		for (const auto& system : systems) system->draw();
	}

	void refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase
			(
				std::remove_if(std::begin(v), std::end(v), [i](Entity* entity)
					{
						return !entity->isActive() || !entity->hasGroup(i);
					}),
				std::end(v)
			);
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

	void addToGroup(Entity* entity, Group group)
	{
		groupedEntities[group].emplace_back(entity);
	}

	std::vector<Entity*>& getGroup(Group group)
	{
		return groupedEntities[group];
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
	std::shared_ptr<Constants> getConstants() { return constants; }

private:
	std::shared_ptr<Constants> constants;
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::shared_ptr<System>> systems;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
	float scale = 1.f;
	int nextID = 0;
};