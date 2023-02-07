#pragma once
#include "Components.h"
#include "Vector2D.h"

struct TransformComponent : public Component
{
	Vector2D position;

	TransformComponent() : position() {}
	TransformComponent(float x, float y) : position(x, y) {}
	void update() override
	{

	}
};