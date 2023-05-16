#pragma once
#include "ECS.h"
#include "SpriteComponent.h"
#include "SpriteOutlineComponent.h"
#include "ColliderComponent.h"
#include "HitboxComponent.h"
#include "WeaponComponent.h"
#include "UILabelComponent.h"

#include "AssetManager.h"
#include "CameraManager.h"
#include "Map.h"

namespace ecs
{
	class RenderSystem : public System
	{
	public:
		explicit RenderSystem(Manager& manager) : System(manager), blackTexture(nullptr) {}
		~RenderSystem() override
		{
			SDL_DestroyTexture(blackTexture);
		}

		void update(double delta) override
		{
			if (!blackTexture)
			{
				blackTexture = TextureManager::getTextureFromSurface(Game::assetManager->getSurface("black"));
			}

			sortedEntities.clear();
			tiles = manager.getGroup(Game::eGroupLabels::MAP);
			const auto entitiesWithCollision = manager.getEntitiesWithComponent<ecs::ColliderComponent>();

			for (auto& entity : entitiesWithCollision)
			{
				const auto colliderComponent = entity->getComponent<ecs::ColliderComponent>();
				const double x = colliderComponent->getCollider()->getPosition().x;
				const double z = colliderComponent->getLowestPoint();
				sortedEntities.push_back({ entity, x, z });
			}

			std::sort(sortedEntities.begin(), sortedEntities.end(),
				[](const EntityZValue& a, const EntityZValue& b)
				{
					if (a.entityZ == b.entityZ) return a.entityX > b.entityX;
					return a.entityZ < b.entityZ;
				});
		}

		void draw() override
		{
			// FIX this
			const auto cameraBounds = Game::cameraManager->getCameraBounds();
			TextureManager::draw(blackTexture, SDL_Rect(0, 0, 1, 1), SDL_Rect(cameraBounds.x, cameraBounds.y, Game::constants->SCREEN_WIDTH, Game::constants->SCREEN_HEIGHT), SDL_FLIP_NONE);

			for (const auto& tile : tiles) tile->draw();
			for (const auto& entityZValue : sortedEntities) entityZValue.entity->draw();

			auto labelEntities = manager.getEntitiesWithComponent<ecs::UILabelComponent>();
			for (const auto& labelEntity : labelEntities) labelEntity->draw();
		}

	private:
		struct EntityZValue
		{
			ecs::Entity* entity;
			double entityX;
			double entityZ;
		};
		std::vector<ecs::Entity*> tiles;
		std::vector<EntityZValue> sortedEntities;

		SDL_Texture* blackTexture; // todo: remove
	};
}