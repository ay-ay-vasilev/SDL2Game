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

			if (playerCollider->collidesWith(*collider))
			{
				float overlapX = playerCollider->getOverlapX(*collider);
				float overlapY = playerCollider->getOverlapY(*collider);

				int overlapXInt = static_cast<int>(overlapX);
				int overlapYInt = static_cast<int>(overlapY);

				if (overlapX < overlapY && overlapX > 0)
				{
					if (playerPosition.x < collider->getPosition().x)
					{
						playerTransformComponent.movePosition(-overlapXInt, 0);
						playerColliderComponent.moveColliderPos(-overlapXInt, 0);
					}
					else
					{
						playerTransformComponent.movePosition(overlapXInt, 0);
						playerColliderComponent.moveColliderPos(overlapXInt, 0);
					}
				}
				else if (overlapX >= overlapY && overlapY > 0)
				{
					if (playerPosition.y < collider->getPosition().y)
					{
						playerTransformComponent.movePosition(0, -overlapYInt);
						playerColliderComponent.moveColliderPos(0, -overlapYInt);
					}
					else
					{
						playerTransformComponent.movePosition(0, overlapYInt);
						playerColliderComponent.moveColliderPos(0, overlapYInt);
					}
				}
			}
		}
	}
}

void CollisionSystem::draw()
{
}