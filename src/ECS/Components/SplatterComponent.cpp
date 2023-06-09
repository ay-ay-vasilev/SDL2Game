#include "SplatterComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ColliderComponent.h"

ecs::SplatterComponent::SplatterComponent() {}

void ecs::SplatterComponent::init()
{
	healthComponent = entity->getComponent<HealthComponent>();
	registerWithSubject(healthComponent);
}

void ecs::SplatterComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "hit") createSplatter();
}

void ecs::SplatterComponent::createSplatter()
{
	needSplatter = true;

	const auto colliderComponent = entity->getComponent<ecs::ColliderComponent>();
	const double x = colliderComponent->getCollider()->getPosition().x + colliderComponent->getCollider()->getWidth() / 2;
	const double y = colliderComponent->getLowestPoint();

	splatterData = SplatterData(Vector2D(x, y));
}