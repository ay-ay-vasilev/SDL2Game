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
	TransformComponent(float scale) : position(Vector2D(400, 320)), scale(scale) {}
	TransformComponent(float x, float y) : position(x, y) {}
	TransformComponent(float x, float y, int w, int h, float scale) : position(x, y), width(w), height(h), scale(scale) {}

	void init() override
	{
		velocity.Zero();
	}
	void update() override
	{
	}
};