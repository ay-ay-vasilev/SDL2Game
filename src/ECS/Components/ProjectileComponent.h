#pragma once

#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(Vector2D velocity, float range) :
		transform(nullptr),
		velocity(velocity),
		range(range) {}
	~ProjectileComponent() {}

	void init() override
	{
		transform = entity->getComponent<TransformComponent>();
		transform->setVeloctiy(velocity);
		speed = transform->getSpeed();
		range *= transform->getScale();
	}

	void update() override
	{
		distance += speed;
		if (distance > range)
		{
			entity->destroy();
			std::cout << "projectile: out of range!\n";
		}
	}

private:
	std::shared_ptr<TransformComponent> transform;
	Vector2D velocity{ 0.f, 0.f };
	float range = 0.f;
	float speed = 0.f;
	float distance = 0.f;
};