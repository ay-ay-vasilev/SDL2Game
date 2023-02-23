#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(Vector2D velocity, int range, int speed) : velocity(velocity), range(range), speed(speed) {}
	~ProjectileComponent() {}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
		transform->speed = speed;
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

	TransformComponent* transform;

	Vector2D velocity{ 0, 0 };
	int range = 0;
	int speed = 0;
	int distance = 0;
};