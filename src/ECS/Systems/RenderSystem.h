#pragma once
#include "ECS.h"
#include "SpriteComponent.h"
#include "SpriteOutlineComponent.h"
#include "ColliderComponent.h"
#include "HitboxComponent.h"
#include "WeaponComponent.h"

#include "AssetManager.h"
#include "Map.h"

class RenderSystem : public System
{
public:
	explicit RenderSystem(Manager& manager) : System(manager) {}

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
				return a.entityZ < b.entityZ;
			});
	}

	void draw() override
	{
		auto blackTexture = TextureManager::getTextureFromSurface(Game::assets->getSurface("black"));
		TextureManager::draw(blackTexture, SDL_Rect(0, 0, 1, 1), SDL_Rect(0, 0, Game::constants->SCREEN_WIDTH, Game::constants->SCREEN_HEIGHT), SDL_FLIP_NONE);

		for (const auto& tile : tiles) tile->draw();

		for (const auto& entityZValue : sortedEntities)
		{
			const auto outlineComponent = entityZValue.entity->getComponent<SpriteOutlineComponent>();
			if (outlineComponent) outlineComponent->draw();

			const auto spriteComponent = entityZValue.entity->getComponent<SpriteComponent>();
			if (spriteComponent) spriteComponent->draw();
		}

		if (Game::constants->DRAW_COLLIDERS)
		{
			for (const auto& entityZValue : sortedEntities)
			{
				const auto colliderComponent = entityZValue.entity->getComponent<ColliderComponent>();
				if (colliderComponent) colliderComponent->draw();
			}
		}

		if (Game::constants->DRAW_HITBOXES)
		{
			for (const auto& entityZValue : sortedEntities)
			{
				const auto hitboxComponent = entityZValue.entity->getComponent<HitboxComponent>();
				if (hitboxComponent) hitboxComponent->draw();

				const auto weaponComponent = entityZValue.entity->getComponent<WeaponComponent>();
				if (weaponComponent) weaponComponent->draw();
			}
		}
	}

private:
	struct EntityZValue
	{
		Entity* entity;
		double entityZ;
	};
	std::vector<Entity*> tiles;
	std::vector<EntityZValue> sortedEntities;
};