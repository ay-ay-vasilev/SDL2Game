#include "CollisionSystem.h"
#include "Collision.h"
#include "Vector2D.h"

#include <cmath>

void CollisionSystem::update()
{
	auto entitiesWithColliders = manager.getEntitiesWithComponent<ColliderComponent>();
	for (auto entity : entitiesWithColliders)
	{
		entity->getComponent<ColliderComponent>()->setDebugDraw(manager.getConstants()->DRAW_COLLIDERS);
	}

	movingEntities = manager.getEntitiesWithComponents<TransformComponent, ColliderComponent>();
	colliderEntities = manager.getGroup(Game::eGroupLabels::COLLIDERS);

	for (auto movingEntity : movingEntities)
	{
		auto movingEntityColliderComponent = movingEntity->getComponent<ColliderComponent>();
		auto movingEntityTransformComponent = movingEntity->getComponent<TransformComponent>();
		auto movingEntityCollider = movingEntityColliderComponent->getCollider();

		for (auto colliderEntity : colliderEntities)
		{
			if (colliderEntity == movingEntity)
				continue;

			const auto colliderEntityCollider = colliderEntity->getComponent<ColliderComponent>()->getCollider();
			float overlapX, overlapY;
			if (overlap(movingEntityCollider, colliderEntityCollider, overlapX, overlapY))
			{
				adjustPosition(overlapX, overlapY, colliderEntityCollider->getPosition(), movingEntityTransformComponent, movingEntityColliderComponent);
			}
		}
	}
}

void CollisionSystem::draw()
{
}

bool CollisionSystem::overlap(const std::shared_ptr<ColliderShape>& collider1, const std::shared_ptr<ColliderShape>& collider2, float& overlapX, float& overlapY) {
	if (!collider1->collidesWith(collider2))
		return false;

	overlapX = collider1->getOverlapX(collider2);
	overlapY = collider1->getOverlapY(collider2);

	return true;
}

void CollisionSystem::adjustPosition(const float overlapX, const float overlapY, const Vector2D& colliderPosition, std::shared_ptr<TransformComponent> transformComponent, std::shared_ptr<ColliderComponent> colliderComponent)
{
	const auto position = transformComponent->getPosition();

	if (overlapX < overlapY && fabs(overlapX - overlapY) > 0.001 && overlapX > 0.001)
	{
		const float amount = (position.x < colliderPosition.x) ? -overlapX : overlapX;
		transformComponent->movePosition(amount, 0.f);
		colliderComponent->moveColliderPos(amount, 0.f);
	}
	else if (overlapX > overlapY && fabs(overlapX - overlapY) > 0.001 && overlapY > 0.001)
	{
		const float amount = (position.y < colliderPosition.y) ? -overlapY : overlapY;
		transformComponent->movePosition(0.f, amount);
		colliderComponent->moveColliderPos(0.f, amount);
	}
	else if (overlapX == overlapY && overlapX > 0.001)
	{
		const float overlap = overlapX;
		const float moveAmount = overlap / 2.f;
		const float xAmount = (position.x < colliderPosition.x) ? -moveAmount : moveAmount;
		const float yAmount = (position.y < colliderPosition.y) ? -moveAmount : moveAmount;
		transformComponent->movePosition(xAmount, yAmount);
		colliderComponent->moveColliderPos(xAmount, yAmount);
	}
}