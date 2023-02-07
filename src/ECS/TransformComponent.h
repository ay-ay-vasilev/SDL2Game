#pragma once
#include "Components.h"
#include "Vector2D.h"

struct TransformComponent : public Component
{
	Vector2D position;
	Vector2D velocity;

	int speed = 3;

	TransformComponent() : position() {}
	TransformComponent(float x, float y) : position(x, y) {}

	void init() override
	{
		velocity = Vector2D(0, 0);
	}
	void update() override
	{
		position += Vector2D(velocity.x * speed, velocity.y * speed);
	}
};