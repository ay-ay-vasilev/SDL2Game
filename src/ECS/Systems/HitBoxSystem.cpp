#include "HitBoxSystem.h"
#include "Collision.h"
#include "Vector2D.h"

void HitBoxSystem::update()
{
	if (manager.getConstants()->DRAW_HITBOXES)
	{
		auto entitiesWithHitBoxes = manager.getEntitiesWithComponent<HitBoxComponent>();
		for (auto entity : entitiesWithHitBoxes)
		{
			entity->getComponent<HitBoxComponent>().setDebugDraw(true);
		}

		auto entitiesWithWeapons = manager.getEntitiesWithComponent<WeaponComponent>();
		for (auto entity : entitiesWithWeapons)
		{
			entity->getComponent<WeaponComponent>().setDebugDraw(true);
		}
	}

	players = manager.getGroup(Game::eGroupLabels::PLAYERS);
	enemies = manager.getGroup(Game::eGroupLabels::ENEMIES);
	projectiles = manager.getGroup(Game::eGroupLabels::PROJECTILES);

	for (auto player : players)
	{
		auto& playerHitBox = player->getComponent<HitBoxComponent>().hitBox;
		auto& playerPosition = player->getComponent<TransformComponent>().position;
		auto& playerVelocity = player->getComponent<TransformComponent>().velocity;

		for (auto projectile : projectiles)
		{
			auto& projectileHitBox = projectile->getComponent<HitBoxComponent>().hitBox;
			if (Collision::AABB(projectileHitBox, playerHitBox))
			{
				projectile->destroy();
				std::cout << "projectile: hit the player!\n";
			}
		}
	}
}

void HitBoxSystem::draw()
{
}