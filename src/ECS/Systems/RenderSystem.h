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
		const auto entitiesWithCollision = manager.getEntitiesWithComponent<ColliderComponent>();

		for (auto& entity : entitiesWithCollision)
		{
			const auto colliderComponent = entity->getComponent<ColliderComponent>();
			const double z = colliderComponent->getLowestPoint();
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
		auto blackTex = Game::assets->getTexture("black");
		TextureManager::draw(blackTex, SDL_Rect(0, 0, 1, 1), SDL_Rect(0, 0, Game::constants->SCREEN_WIDTH, Game::constants->SCREEN_HEIGHT), SDL_FLIP_NONE);

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