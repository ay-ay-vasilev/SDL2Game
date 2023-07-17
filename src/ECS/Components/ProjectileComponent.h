#pragma once
#include "ECS.h"
#include "Vector2D.h"
#include "Observer.h"

namespace ecs
{
	class TransformComponent;
	class DamageColliderComponent;
	class HealthComponent;
	class ParticleControllerComponent;
	class SpriteComponent;
	class ProjectileComponent : public Component, public Observer
	{
	public:
		ProjectileComponent(const int ownerEntityId, Vector2D velocity, float range, bool velocityRotation);
		
		// Component
		void init() override;
		void update(double delta) override;

		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		void destroy();
		const int inline getOwnerEntityId() const { return ownerEntityId; }

	private:
		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<DamageColliderComponent> damageCollider;
		std::shared_ptr<HealthComponent> healthComponent;
		std::shared_ptr<SpriteComponent> spriteComponent;
		std::shared_ptr<ParticleControllerComponent> particleControllerComponent;
		int ownerEntityId;

		Vector2D velocity{ 0.f, 0.f };
		float range{ 0.f };
		float speed{ 0.f };
		float distance{ 0.f };
		float angle{ 0.f };
		bool velocityRotation = false;
		bool destroyed = false;
	};
}