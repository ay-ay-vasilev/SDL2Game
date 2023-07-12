#include "ProjectileComponent.h"
#include "TransformComponent.h"
#include "DamageColliderComponent.h"

ecs::ProjectileComponent::ProjectileComponent(const int ownerEntityId, Vector2D velocity, float range, bool velocityRotation) :
	ownerEntityId(ownerEntityId),
	transform(nullptr),
	velocity(velocity),
	range(range),
	velocityRotation(velocityRotation) {}

void ecs::ProjectileComponent::init()
{
	transform = entity->getComponent<ecs::TransformComponent>();
	damageCollider = entity->getComponent<ecs::DamageColliderComponent>();
	transform->setVeloctiy(velocity);
	angle = Vector2D::Angle(velocity) + 180.0;
	if (velocityRotation) transform->setRotation(angle);
	speed = transform->getSpeed();
	range *= transform->getScale();

	const float distance = damageCollider->getOffset().Distance();
	const float offsetX = distance * std::cos((angle - 180.0) * M_PI / 180.0);
	const float offsetY = distance * std::sin((angle - 180.0) * M_PI / 180.0);

	damageCollider->setOffset({offsetX, offsetY});
}

void ecs::ProjectileComponent::update(double delta)
{
	distance += speed;
	if (distance > range)
	{
		std::cout << "projectile: out of range!\n";
		entity->destroy();
	}
}
