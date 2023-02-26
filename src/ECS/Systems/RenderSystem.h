#pragma once
#include "Components.h"
#include "Map.h"

class RenderSystem : public System
{
public:
	RenderSystem(Manager& manager) : System(manager) {}

	void update() override
	{
		sortedEntities.clear();
		tiles = manager.getGroup(Game::eGroupLabels::MAP);
		auto entitiesWithTransform = manager.getEntitiesWithComponent<TransformComponent>();

		for (auto& entity : entitiesWithTransform)
		{
			auto transform = entity->getComponent<TransformComponent>();
			double z = transform.position.y + transform.getScaledHeight();
			sortedEntities.push_back({ entity, z });
		}

		std::sort(sortedEntities.begin(), sortedEntities.end(),
			[](const EntityZValue& a, const EntityZValue& b)
			{
				return a.z < b.z;
			});
	}

	void draw() override
	{
		for (const auto& tile : tiles) tile->draw();
		for (const auto& entityZValue : sortedEntities) entityZValue.entity->draw();
	}

private:
	struct EntityZValue
	{
		Entity* entity;
		double z;
	};
	std::vector<Entity*> tiles;
	std::vector<EntityZValue> sortedEntities;
};