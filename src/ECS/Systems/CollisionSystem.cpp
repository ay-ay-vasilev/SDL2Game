#include "CollisionSystem.h"
#include "Collision.h"
#include "Vector2D.h"

void CollisionSystem::update()
{
	auto entitiesWithColliders = manager.getEntitiesWithComponent<ColliderComponent>();
	for (auto entity : entitiesWithColliders)
	{
		entity->getComponent<ColliderComponent>().setDebugDraw(manager.getConstants()->DRAW_COLLIDERS);
	}

	players = manager.getGroup(Game::eGroupLabels::PLAYERS);
	colliders = manager.getGroup(Game::eGroupLabels::COLLIDERS);

	for (auto player : players)
	{
		auto& playerColliderComponent = player->getComponent<ColliderComponent>();
		auto& playerTransformComponent = player->getComponent<TransformComponent>();

		for (auto c : colliders)
		{
			if (c == player)
			{
				continue;
			}

			const auto playerCollider = playerColliderComponent.getCollider();
			const auto playerPosition = playerTransformComponent.getPosition();

			const auto collider = c->getComponent<ColliderComponent>().getCollider();
			if (Collision::AABB(collider, playerCollider))
			{
				int overlapXInt = std::min(playerCollider.x + playerCollider.w, collider.x + collider.w) - std::max(playerCollider.x, collider.x);
				int overlapYInt = std::min(playerCollider.y + playerCollider.h, collider.y + collider.h) - std::max(playerCollider.y, collider.y);

				float overlapX = static_cast<float>(overlapXInt);
				float overlapY = static_cast<float>(overlapYInt);

				if (overlapX < overlapY && overlapX > 0)
				{
					if (playerPosition.x < collider.x)
					{
						playerTransformComponent.setPosition(playerPosition.x - overlapXInt, playerPosition.y);
						playerColliderComponent.setColliderPos(playerCollider.x - overlapXInt, playerCollider.y);
					}
					else
					{
						playerTransformComponent.setPosition(playerPosition.x + overlapXInt, playerPosition.y);
						playerColliderComponent.setColliderPos(playerCollider.x + overlapXInt, playerCollider.y);
					}
				}
				else if (overlapX >= overlapY && overlapY > 0)
				{
					if (playerPosition.y < collider.y)
					{
						playerTransformComponent.setPosition(playerPosition.x, playerPosition.y - overlapYInt);
						playerColliderComponent.setColliderPos(playerCollider.x, playerCollider.y - overlapYInt);
					}
					else
					{
						playerTransformComponent.setPosition(playerPosition.x, playerPosition.y + overlapYInt);
						playerColliderComponent.setColliderPos(playerCollider.x, playerCollider.y + overlapYInt);
					}
				}
			}
		}
	}
}

void CollisionSystem::draw()
{
}