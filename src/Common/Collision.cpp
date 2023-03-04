#include "Collision.h"
#include "ColliderComponent.h"
#include "ColliderShape.h"

bool Collision::collides(const RectangleCollider& rectA, const RectangleCollider& rectB)
{
	if
		(
			(rectA.getPosition().x + rectA.getWidth() > rectB.getPosition().x) &&
			(rectB.getPosition().x + rectB.getWidth() > rectA.getPosition().x) &&
			(rectA.getPosition().y + rectA.getHeight() > rectB.getPosition().y) &&
			(rectB.getPosition().y + rectB.getHeight() > rectA.getPosition().y)
		)
		return true;
	return false;
}

bool Collision::collides(const RectangleCollider& rect, const CircleCollider& circle)
{
	float closestX = clamp(circle.getPosition().x, rect.getPosition().x, rect.getPosition().x + rect.getWidth());
	float closestY = clamp(circle.getPosition().y, rect.getPosition().y, rect.getPosition().y + rect.getHeight());
	float distanceX = circle.getPosition().x - closestX;
	float distanceY = circle.getPosition().y - closestY;
	return (distanceX * distanceX + distanceY * distanceY) < (circle.getRadius() * circle.getRadius());
}

bool Collision::collides(const CircleCollider& circle, const RectangleCollider& rect)
{
	return collides(rect, circle);
}

bool Collision::collides(const CircleCollider& circleA, const CircleCollider& circleB)
{
	float distanceX = circleA.getPosition().x - circleB.getPosition().x;
	float distanceY = circleA.getPosition().y - circleB.getPosition().y;
	float radiusSum = circleA.getWidth() + circleB.getWidth();
	return (distanceX * distanceX + distanceY * distanceY) < (radiusSum * radiusSum);
}

float Collision::clamp(float value, float min, float max)
{
	if (value < min) { return min; }
	else if (value > max) { return max; }
	else { return value; }
}

float Collision::overlapX(const RectangleCollider& rectA, const RectangleCollider& rectB) {
	float overlap = 0.0f;
	float leftA = rectA.getPosition().x;
	float rightA = leftA + rectA.getWidth();
	float leftB = rectB.getPosition().x;
	float rightB = leftB + rectB.getWidth();
	if (leftA < rightB && rightA > leftB) {
		overlap = std::min(rightA, rightB) - std::max(leftA, leftB);
	}
	return overlap;
}

float Collision::overlapX(const RectangleCollider& rect, const CircleCollider& circle) {
	float overlap = 0.0f;
	float left = rect.getPosition().x;
	float right = left + rect.getWidth();
	float circleLeft = circle.getPosition().x - circle.getRadius();
	float circleRight = circle.getPosition().x + circle.getRadius();
	if (left < circleRight && right > circleLeft) {
		overlap = std::min(right, circleRight) - std::max(left, circleLeft);
	}
	return overlap;
}

float Collision::overlapX(const CircleCollider& circle, const RectangleCollider& rect) {
	return Collision::overlapX(rect, circle);
}

float Collision::overlapX(const CircleCollider& circle1, const CircleCollider& circle2) {
	float overlap = 0.0f;
	float distance = circle1.getPosition().x - circle2.getPosition().x;
	float radiusSum = circle1.getRadius() + circle2.getRadius();
	if (std::abs(distance) < radiusSum) {
		overlap = radiusSum - std::abs(distance);
	}
	return overlap;
}

float Collision::overlapY(const RectangleCollider& rectA, const RectangleCollider& rectB) {
	float overlap = 0.0f;
	float topA = rectA.getPosition().y;
	float bottomA = topA + rectA.getHeight();
	float topB = rectB.getPosition().y;
	float bottomB = topB + rectB.getHeight();
	if (topA < bottomB && bottomA > topB) {
		overlap = std::min(bottomA, bottomB) - std::max(topA, topB);
	}
	return overlap;
}

float Collision::overlapY(const RectangleCollider& rect, const CircleCollider& circle) {
	float overlap = 0.0f;
	float top = rect.getPosition().y;
	float bottom = top + rect.getHeight();
	float circleTop = circle.getPosition().y - circle.getRadius();
	float circleBottom = circle.getPosition().y + circle.getRadius();
	if (top < circleBottom && bottom > circleTop) {
		overlap = std::min(bottom, circleBottom) - std::max(top, circleTop);
	}
	return overlap;
}

float Collision::overlapY(const CircleCollider& circle, const RectangleCollider& rect) {
	return Collision::overlapY(rect, circle);
}

float Collision::overlapY(const CircleCollider& circle1, const CircleCollider& circle2) {
	float overlap = 0.0f;
	float distance = circle1.getPosition().y - circle2.getPosition().y;
	float radiusSum = circle1.getRadius() + circle2.getRadius();
	if (std::abs(distance) < radiusSum) {
		overlap = radiusSum - std::abs(distance);
	}
	return overlap;
}