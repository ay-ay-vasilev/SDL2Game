#pragma once
#include "ECS.h"
#include "SpriteComponent.h"
#include "SpriteOutlineComponent.h"
#include "ColliderComponent.h"
#include "HitboxComponent.h"
#include "WeaponComponent.h"
#include "CorpseComponent.h"
#include "UILabelComponent.h"

#include "AssetManager.h"
#include "CameraManager.h"
#include "Map.h"

namespace ecs
{
	class RenderSystem : public System
	{
	public:
		explicit RenderSystem(Manager& manager) : System(manager) {}

		void init() override {}

		void update(double delta) override
		{
			sortedEntities.clear();
			tiles = manager.getGroup(Game::eGroupLabels::MAP);
			const auto entitiesWithCollision = manager.getEntitiesWithComponent<ecs::ColliderComponent>();

			for (auto& entity : entitiesWithCollision)
			{
				const auto colliderComponent = entity->getComponent<ecs::ColliderComponent>();
				const double x = colliderComponent->getCollider()->getPosition().x;
				const double z = colliderComponent->getLowestPoint();

				bool background = false;
				const auto corpseComponent = entity->getComponent<ecs::CorpseComponent>();
				if (corpseComponent)
				{
					if (corpseComponent->isCorpse()) background = true;
				}
				sortedEntities.push_back({ entity, x, z, background });
			}

			std::sort(sortedEntities.begin(), sortedEntities.end(),
				[](const EntityZValue& a, const EntityZValue& b)
				{
					if (a.background && !b.background) return true;
					if (b.background && !a.background) return false;
					if (a.entityZ == b.entityZ) return a.entityX > b.entityX;
					return a.entityZ < b.entityZ;
				});
		}

		void draw() override
		{
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
			bool background;
		};
		std::vector<ecs::Entity*> tiles;
		std::vector<EntityZValue> sortedEntities;
	};
}