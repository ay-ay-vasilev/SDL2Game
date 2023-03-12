#include "ColliderShape.h"

bool RectangleCollider::collidesWith(const std::shared_ptr<ColliderShape>& other) const
{
	if (auto otherRect = std::dynamic_pointer_cast<const RectangleCollider>(other)) { return Collision::collides(*this, *otherRect); }
	else if (auto otherCircle = std::dynamic_pointer_cast<const CircleCollider>(other)) { return Collision::collides(*this, *otherCircle); }
	else { return false; }
}

float RectangleCollider::getOverlapX(const std::shared_ptr<ColliderShape>& other) const
{
	if (auto otherRect = std::dynamic_pointer_cast<const RectangleCollider>(other)) { return Collision::overlapX(*this, *otherRect); }
	else if (auto otherCircle = std::dynamic_pointer_cast<const CircleCollider>(other)) { return Collision::overlapX(*this, *otherCircle); }
	else { return 0.f; }
}

float RectangleCollider::getOverlapY(const std::shared_ptr<ColliderShape>& other) const
{
	if (auto otherRect = std::dynamic_pointer_cast<const RectangleCollider>(other)) { return Collision::overlapY(*this, *otherRect); }
	else if (auto otherCircle = std::dynamic_pointer_cast<const CircleCollider>(other)) { return Collision::overlapY(*this, *otherCircle); }
	else { return 0.f; }
}

bool CircleCollider::collidesWith(const std::shared_ptr<ColliderShape>& other) const
{
	if (auto otherCircle = std::dynamic_pointer_cast<const CircleCollider>(other)) { return Collision::collides(*this, *otherCircle); }
	else if (auto otherRect = std::dynamic_pointer_cast<const RectangleCollider>(other)) { return Collision::collides(*this, *otherRect); }
	else { return false; }
}

float CircleCollider::getOverlapX(const std::shared_ptr<ColliderShape>& other) const
{
	if (auto otherRect = std::dynamic_pointer_cast<const RectangleCollider>(other)) { return Collision::overlapX(*this, *otherRect); }
	else if (auto otherCircle = std::dynamic_pointer_cast<const CircleCollider>(other)) { return Collision::overlapX(*this, *otherCircle); }
	else { return 0.f; }
}

float CircleCollider::getOverlapY(const std::shared_ptr<ColliderShape>& other) const
{
	if (auto otherRect = std::dynamic_pointer_cast<const RectangleCollider>(other)) { return Collision::overlapY(*this, *otherRect); }
	else if (auto otherCircle = std::dynamic_pointer_cast<const CircleCollider>(other)) { return Collision::overlapY(*this, *otherCircle); }
	else { return 0.f; }
}
