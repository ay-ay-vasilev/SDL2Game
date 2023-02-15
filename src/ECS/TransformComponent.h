#pragma once
#include "Components.h"
#include "Vector2D.h"

struct TransformComponent : public Component
{
	Vector2D position;
	Vector2D velocity;

	int width = 16;
	int height = 16;
	float scale = 1.f;

	int speed = 5;

	TransformComponent() : position() {}
	TransformComponent(const float scale) : position(Vector2D(400, 320)), scale(scale) {}
	TransformComponent(const float x, const float y) : position(x, y) {}
	TransformComponent(const float x, const float y, const int w, const int h, const float scale) : position(x, y), width(w), height(h), scale(scale) {}

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