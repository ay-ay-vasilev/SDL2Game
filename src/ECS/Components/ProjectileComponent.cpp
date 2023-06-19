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
	transform->setRotation(Vector2D::Angle(velocity) + 180.0);
	speed = transform->getSpeed();
	range *= transform->getScale();
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
