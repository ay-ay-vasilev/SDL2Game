#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "ColliderShape.h"

namespace ecs
{
	class CollisionSystem : public System
	{
	public:
		explicit CollisionSystem(Manager& manager) : System(manager) {}

		void update() override;
		void draw() override;

		bool overlap(const std::shared_ptr<ColliderShape>& collider1, const std::shared_ptr<ColliderShape>& collider2, float& overlapX, float& overlapY);
		void adjustPosition(const float overlapX, const float overlapY, const Vector2D& colliderPosition, std::shared_ptr<ecs::TransformComponent> transformComponent, std::shared_ptr<ecs::ColliderComponent> colliderComponent);
	private:
		std::vector<ecs::Entity*> movingEntities;
		std::vector<ecs::Entity*> colliderEntities;
	};
}