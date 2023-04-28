#include "ProjectileComponent.h"
#include "TransformComponent.h"

ecs::ProjectileComponent::ProjectileComponent(Vector2D velocity, float range) :
	transform(nullptr),
	velocity(velocity),
	range(range) {}

void ecs::ProjectileComponent::init()
{
	transform = entity->getComponent<ecs::TransformComponent>();
	transform->setVeloctiy(velocity);
	speed = transform->getSpeed();
	range *= transform->getScale();
}

void ecs::ProjectileComponent::update()
{
	distance += speed;
	if (distance > range)
	{
		entity->destroy();
		std::cout << "projectile: out of range!\n";
	}
}
