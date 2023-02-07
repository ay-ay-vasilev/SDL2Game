#pragma once
#include "Components.h"
#include "Vector2D.h"

struct TransformComponent : public Component
{
	Vector2D position;
	Vector2D velocity;

	int width = 32;
	int height = 32;
	float scale = 1.f;

	int speed = 3;

	TransformComponent() : position() {}
	TransformComponent(float scale) : position(), scale(scale) {}
	TransformComponent(float x, float y) : position(x, y) {}
	TransformComponent(float x, float y, int w, int h, float scale) : position(x, y), width(w), height(h), scale(scale) {}

	void init() override
	{
		velocity.Zero();
	}
	void update() override
	{
		int newX = static_cast<int>(velocity.x * speed);
		int newY = static_cast<int>(velocity.y * speed);

		position += Vector2D(std::clamp(newX, -speed, speed), std::clamp(newY, -speed, speed));
	}
};