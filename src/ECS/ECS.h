#pragma once

#include "Constants.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
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
using ComponentArray = std::array<Component*, maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;

class Component
{
public:
	Entity* entity;
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
public:
	Entity(Manager& manager) : manager(manager) {}
	void update()
	{
		for (auto& c : components) c->update();
	}
	void draw() 
	{
		for (auto& c : components) c->draw();
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

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

private:
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;
};

class System
{
public:
	System(Manager& manager) : manager(manager) {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~System() {}

protected:
	Manager& manager;
};

class Manager
{
public:
	void update()
	{
		for (const auto& e : entities) e->update();
		for (const auto& s : systems) s->update();
	}
	
	void draw()
	{
		/*for (const auto& e : entities) e->draw();*/
		for (const auto& s : systems) s->draw();
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
		auto e = std::make_unique<Entity>(*this);
		entities.emplace_back(std::move(e));
		return *entities.back();
	}

	template <typename T>
	std::shared_ptr<T> addSystem()
	{
		std::shared_ptr<T> system = std::make_shared<T>(*this);
		systems.emplace_back(system);
		return system;
	}

	template <typename T>
	std::vector<Entity*> getEntitiesWithComponent()
	{
		std::vector<Entity*> entitiesWithComponent;
		for (const auto& e : entities) {
			if (e->hasComponent<T>()) {
				entitiesWithComponent.push_back(e.get());
			}
		}
		return entitiesWithComponent;
	}

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::shared_ptr<System>> systems;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
};