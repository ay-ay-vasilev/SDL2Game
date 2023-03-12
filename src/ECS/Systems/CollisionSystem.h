#pragma once
#include "Components.h"

class CollisionSystem : public System
{
public:
	CollisionSystem(Manager& manager) : System(manager) {}

	void update() override;
	void draw() override;

	bool overlap(const std::shared_ptr<ColliderShape>& collider1, const std::shared_ptr<ColliderShape>& collider2, float& overlapX, float& overlapY);
	void adjustPosition(const float overlapX, const float overlapY, const Vector2D& colliderPosition, std::shared_ptr<TransformComponent> transformComponent, std::shared_ptr<ColliderComponent> colliderComponent);
private:
	std::vector<Entity*> movingEntities;
	std::vector<Entity*> colliderEntities;
};