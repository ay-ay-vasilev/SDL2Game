#include "ProjectileComponent.h"
#include "TransformComponent.h"

ProjectileComponent::ProjectileComponent(Vector2D velocity, float range) :
	transform(nullptr),
	velocity(velocity),
	range(range) {}

void ProjectileComponent::init()
{
	transform = entity->getComponent<TransformComponent>();
	transform->setVeloctiy(velocity);
	speed = transform->getSpeed();
	range *= transform->getScale();
}

void ProjectileComponent::update()
{
	distance += speed;
	if (distance > range)
	{
		entity->destroy();
		std::cout << "projectile: out of range!\n";
	}
}
