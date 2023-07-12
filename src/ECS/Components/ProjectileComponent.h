#pragma once
#include "ECS.h"
#include "Vector2D.h"

namespace ecs
{
	class TransformComponent;
	class DamageColliderComponent;
	class ProjectileComponent : public Component
	{
	public:
		ProjectileComponent(const int ownerEntityId, Vector2D velocity, float range, bool velocityRotation);
		
		// Component
		void init() override;
		void update(double delta) override;

		const int inline getOwnerEntityId() const { return ownerEntityId; }

	private:
		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<DamageColliderComponent> damageCollider;
		int ownerEntityId;

		Vector2D velocity{ 0.f, 0.f };
		float range{ 0.f };
		float speed{ 0.f };
		float distance{ 0.f };
		float angle{ 0.f };
		bool velocityRotation = false;
	};
}