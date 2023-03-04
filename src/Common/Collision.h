#pragma once
#include <SDL.h>

class ColliderComponent;
class RectangleCollider;
class CircleCollider;

class Collision
{
public:
	static bool collides(const RectangleCollider& rectA, const RectangleCollider& rectB);
	static bool collides(const RectangleCollider& rect, const CircleCollider& circle);
	static bool collides(const CircleCollider& circle, const RectangleCollider& rect);
	static bool collides(const CircleCollider& circle1, const CircleCollider& circle2);

	static float clamp(float value, float min, float max);

	static float overlapX(const RectangleCollider& rectA, const RectangleCollider& rectB);
	static float overlapX(const RectangleCollider& rect, const CircleCollider& circle);
	static float overlapX(const CircleCollider& circle, const RectangleCollider& rect);
	static float overlapX(const CircleCollider& circle1, const CircleCollider& circle2);
	static float overlapY(const RectangleCollider& rectA, const RectangleCollider& rectB);
	static float overlapY(const RectangleCollider& rect, const CircleCollider& circle);
	static float overlapY(const CircleCollider& circle, const RectangleCollider& rect);
	static float overlapY(const CircleCollider& circle1, const CircleCollider& circle2);
};