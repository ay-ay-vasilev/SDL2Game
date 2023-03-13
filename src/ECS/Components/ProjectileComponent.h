#pragma once

#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(Vector2D velocity, float range, float speed) :
		transform(nullptr),
		velocity(velocity),
		range(range),
		speed(speed) {}
	~ProjectileComponent() {}

	void init() override
	{
		transform = entity->getComponent<TransformComponent>();
		transform->setVeloctiy(velocity);
		transform->setSpeed(speed);
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
	Vector2D velocity{ 0, 0 };
	float range = 0;
	float speed = 0;
	float distance = 0;
};