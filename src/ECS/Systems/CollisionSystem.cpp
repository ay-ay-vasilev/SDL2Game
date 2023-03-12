#include "CollisionSystem.h"
#include "Collision.h"
#include "Vector2D.h"

void CollisionSystem::update()
{
	auto entitiesWithColliders = manager.getEntitiesWithComponent<ColliderComponent>();
	for (auto entity : entitiesWithColliders)
	{
		entity->getComponent<ColliderComponent>()->setDebugDraw(manager.getConstants()->DRAW_COLLIDERS);
	}

	movingColliders = manager.getEntitiesWithComponents<TransformComponent, ColliderComponent>();

	colliders = manager.getGroup(Game::eGroupLabels::COLLIDERS);

	for (auto movingCollider : movingColliders)
	{
		auto playerColliderComponent = movingCollider->getComponent<ColliderComponent>();
		auto playerTransformComponent = movingCollider->getComponent<TransformComponent>();

		for (auto c : colliders)
		{
			if (c == movingCollider)
			{
				continue;
			}

			const auto playerCollider = playerColliderComponent->getCollider();
			const auto playerPosition = playerTransformComponent->getPosition();

			const auto collider = c->getComponent<ColliderComponent>()->getCollider();

			if (playerCollider->collidesWith(collider))
			{
				float overlapX = playerCollider->getOverlapX(collider);
				float overlapY = playerCollider->getOverlapY(collider);

				if (overlapX < overlapY && overlapX > 0.001)
				{
					if (playerPosition.x < collider->getPosition().x)
					{
						playerTransformComponent->movePosition(-overlapX, 0.f);
						playerColliderComponent->moveColliderPos(-overlapX, 0.f);
					}
					else
					{
						playerTransformComponent->movePosition(overlapX, 0.f);
						playerColliderComponent->moveColliderPos(overlapX, 0.f);
					}
				}
				else if (overlapX >= overlapY && overlapY > 0)
				{
					if (playerPosition.y < collider->getPosition().y)
					{
						playerTransformComponent->movePosition(0.f, -overlapY);
						playerColliderComponent->moveColliderPos(0.f, -overlapY);
					}
					else
					{
						playerTransformComponent->movePosition(0.f, overlapY);
						playerColliderComponent->moveColliderPos(0.f, overlapY);
					}
				}
			}
		}
	}
}

void CollisionSystem::draw()
{
}