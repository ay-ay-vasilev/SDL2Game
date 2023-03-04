#include "ColliderShape.h"

bool RectangleCollider::collidesWith(const ColliderShape& other) const
{
	if (auto otherRect = dynamic_cast<const RectangleCollider*>(&other)) { return Collision::collides(*this, *otherRect); }
	else if (auto otherCircle = dynamic_cast<const CircleCollider*>(&other)) { return Collision::collides(*this, *otherCircle); }
	else { return false; }
}

float RectangleCollider::getOverlapX(const ColliderShape& other) const
{
	if (auto otherRect = dynamic_cast<const RectangleCollider*>(&other)) { return Collision::overlapX(*this, *otherRect); }
	else if (auto otherCircle = dynamic_cast<const CircleCollider*>(&other)) { return Collision::overlapX(*this, *otherCircle); }
	else { return 0.f; }
}

float RectangleCollider::getOverlapY(const ColliderShape& other) const
{
	if (auto otherRect = dynamic_cast<const RectangleCollider*>(&other)) { return Collision::overlapY(*this, *otherRect); }
	else if (auto otherCircle = dynamic_cast<const CircleCollider*>(&other)) { return Collision::overlapY(*this, *otherCircle); }
	else { return 0.f; }
}

bool CircleCollider::collidesWith(const ColliderShape& other) const
{
	if (auto otherCircle = dynamic_cast<const CircleCollider*>(&other)) { return Collision::collides(*this, *otherCircle); }
	else if (auto otherRect = dynamic_cast<const RectangleCollider*>(&other)) { return Collision::collides(*this, *otherRect); }
	else { return false; }
}

float CircleCollider::getOverlapX(const ColliderShape& other) const
{
	if (auto otherRect = dynamic_cast<const RectangleCollider*>(&other)) { return Collision::overlapX(*this, *otherRect); }
	else if (auto otherCircle = dynamic_cast<const CircleCollider*>(&other)) { return Collision::overlapX(*this, *otherCircle); }
	else { return 0.f; }
}

float CircleCollider::getOverlapY(const ColliderShape& other) const
{
	if (auto otherRect = dynamic_cast<const RectangleCollider*>(&other)) { return Collision::overlapY(*this, *otherRect); }
	else if (auto otherCircle = dynamic_cast<const CircleCollider*>(&other)) { return Collision::overlapY(*this, *otherCircle); }
	else { return 0.f; }
}
