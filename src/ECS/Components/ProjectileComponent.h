#pragma once

#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(Vector2D velocity, int range, int speed) :
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
	int range = 0;
	int speed = 0;
	int distance = 0;
};