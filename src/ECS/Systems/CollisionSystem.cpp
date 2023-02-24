#include "CollisionSystem.h"
#include "Collision.h"
#include "Vector2D.h"

void CollisionSystem::init()
{
}

void CollisionSystem::update()
{
	players = Game::manager->getGroup(Game::eGroupLabels::PLAYERS);
	colliders = Game::manager->getGroup(Game::eGroupLabels::COLLIDERS);
	projectiles = Game::manager->getGroup(Game::eGroupLabels::PROJECTILES);

	for (auto player : players)
	{
		auto& playerCollider = player->getComponent<ColliderComponent>().collider;
		auto& playerPosition = player->getComponent<TransformComponent>().position;
		auto& playerVelocity = player->getComponent<TransformComponent>().velocity;

		for (auto c : colliders)
		{
			if (c == player)
			{
				continue;
			}

			auto& collider = c->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(collider, playerCollider))
			{
				float overlapX = std::min(playerCollider.x + playerCollider.w, collider.x + collider.w) - std::max(playerCollider.x, collider.x);
				float overlapY = std::min(playerCollider.y + playerCollider.h, collider.y + collider.h) - std::max(playerCollider.y, collider.y);

				if (overlapX < overlapY && overlapX > 0)
				{
					if (playerPosition.x < collider.x)
					{
						playerPosition.x -= overlapX;
						playerCollider.x -= overlapX;
					}
					else
					{
						playerPosition.x += overlapX;
						playerCollider.x += overlapX;
					}
				}
				else if (overlapX >= overlapY && overlapY > 0)
				{
					if (playerPosition.y < collider.y)
					{
						playerPosition.y -= overlapY;
						playerCollider.y -= overlapY;
					}
					else
					{
						playerPosition.y += overlapY;
						playerCollider.y += overlapY;
					}
				}
			}
		}

		for (auto projectile : projectiles)
		{
			auto& collider = projectile->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(collider, playerCollider))
			{
				projectile->destroy();
				std::cout << "projectile: hit the player!\n";
			}
		}
	}
}

void CollisionSystem::draw()
{
	// todo add debug option ?
	for (const auto& c : colliders) c->draw();
}