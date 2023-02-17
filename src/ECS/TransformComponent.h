#pragma once
#include "Components.h"
#include "Vector2D.h"

struct TransformComponent : public Component
{
	Vector2D position;
	Vector2D velocity;

	int width = 0;
	int height = 0;
	float scale = 0;

	int speed = 0;

	TransformComponent() : position() {}
	TransformComponent(const float x, const float y) : position(x, y) {}
	TransformComponent(const float x, const float y, const int w, const int h, const float scale, const int speed = 0) :
		position(x, y), width(w), height(h), scale(scale), speed(speed) {}

	void init() override
	{
		velocity.Zero();
	}
	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};