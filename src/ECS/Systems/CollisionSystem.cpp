#include "CollisionSystem.h"
#include "ColliderComponent.h"
#include "HealthComponent.h"
#include "Collision.h"
#include "Vector2D.h"
#include "Game.h"

#include <cmath>

void ecs::CollisionSystem::update(double delta)
{
	auto entitiesWithColliders = manager.getEntitiesWithComponent<ecs::ColliderComponent>();

	for (const auto entityWithCollider : entitiesWithColliders)
	{
		const auto colliderComponent = entityWithCollider->getComponent<ecs::ColliderComponent>();
		colliderComponent->setEnableDraw(Game::constants->DRAW_COLLIDERS);
	}

	movingEntities = manager.getEntitiesWithComponents<ecs::TransformComponent, ecs::ColliderComponent>();
	colliderEntities = manager.getGroup(Game::eGroupLabels::COLLIDERS);

	for (auto movingEntity : movingEntities)
	{
		auto movingEntityHealth = movingEntity->getComponent<ecs::HealthComponent>();
		if (movingEntityHealth && movingEntityHealth->isDead()) continue;

		auto movingEntityColliderComponent = movingEntity->getComponent<ecs::ColliderComponent>();
		if (!movingEntityColliderComponent->isEnabled()) continue;

		auto movingEntityTransformComponent = movingEntity->getComponent<ecs::TransformComponent>();
		auto movingEntityCollider = movingEntityColliderComponent->getCollider();

		// collision with static colliders
		for (auto colliderEntity : colliderEntities)
		{
			if (colliderEntity == movingEntity)
				continue;

			auto colliderEntityHealth = colliderEntity->getComponent<ecs::HealthComponent>();
			if (colliderEntityHealth && colliderEntityHealth->isDead()) continue;

			auto colliderEntityColliderComponent = colliderEntity->getComponent<ecs::ColliderComponent>();
			if (!colliderEntityColliderComponent->isEnabled()) continue;

			const auto colliderEntityCollider = colliderEntityColliderComponent->getCollider();

			float overlapX, overlapY;
			if (overlap(movingEntityCollider, colliderEntityCollider, overlapX, overlapY))
			{
				adjustPosition(overlapX, overlapY, colliderEntityCollider->getPosition(), movingEntityTransformComponent, movingEntityColliderComponent);
			}
		}
		// collision with moving actors (disabled for player for now)
		if (movingEntity->hasGroup(Game::eGroupLabels::PLAYERS)) continue;
		for (auto otherMovingEntity : movingEntities)
		{
			if (otherMovingEntity == movingEntity)
				continue;

			auto otherMovingEntityHealth = otherMovingEntity->getComponent<ecs::HealthComponent>();
			if (otherMovingEntityHealth && otherMovingEntityHealth->isDead()) continue;

			if (otherMovingEntity->hasGroup(Game::eGroupLabels::PLAYERS))
				continue;

			const auto colliderEntityCollider = otherMovingEntity->getComponent<ecs::ColliderComponent>()->getCollider();
			float overlapX, overlapY;
			if (overlap(movingEntityCollider, colliderEntityCollider, overlapX, overlapY))
			{
				adjustPosition(overlapX, overlapY, colliderEntityCollider->getPosition(), movingEntityTransformComponent, movingEntityColliderComponent);
			}
		}
	}
}

void ecs::CollisionSystem::draw()
{
}

bool ecs::CollisionSystem::overlap(const std::shared_ptr<ColliderShape>& collider1, const std::shared_ptr<ColliderShape>& collider2, float& overlapX, float& overlapY) {
	if (!collider1->collidesWith(collider2))
		return false;

	overlapX = collider1->getOverlapX(collider2);
	overlapY = collider1->getOverlapY(collider2);

	return true;
}

void ecs::CollisionSystem::adjustPosition(const float overlapX, const float overlapY, const Vector2D& colliderPosition, std::shared_ptr<ecs::TransformComponent> transformComponent, std::shared_ptr<ecs::ColliderComponent> colliderComponent)
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