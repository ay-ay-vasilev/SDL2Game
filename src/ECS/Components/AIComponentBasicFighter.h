#pragma once
#include "ECS.h"
#include "Observer.h"
#include "AIComponent.h"

namespace ecs
{
	class ActorComponent;
	class TransformComponent;
	class HitboxComponent;
	class HealthComponent;
	class DamageColliderComponent;
	class AIComponentBasicFighter : public AIComponent, public Observer
	{
	public:
		AIComponentBasicFighter();

		// Component
		void init() override;
		void update(double delta) override;

		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		void setNewTarget(const Entity* newTarget);
		void resetTarget();
		void loseTarget();
		void targetDestroyed();

		const inline float getDistance() const { return distance; }
		const inline float getAggroDistance() const { return aggroDistance; }
		const inline float getLoseAggroDistance() const { return loseAggroDistance; }
		const inline bool hasTarget() const { return target != nullptr; }


	private:
		enum class eState
		{
			IDLE,
			WALK,
			ATTACK
		};

		const Entity* target;
		int targetID;

		std::shared_ptr<ActorComponent> actorComponent;
		std::shared_ptr<HealthComponent> health;
		std::shared_ptr<DamageColliderComponent> damageCollider;

		std::shared_ptr<HealthComponent> targetHealth;
		std::shared_ptr<TransformComponent> targetTransform;
		std::shared_ptr<HitboxComponent> targetHitbox;
		std::shared_ptr<TransformComponent> transform;

		eState state;
		float distance;
		float aggroDistance;
		float loseAggroDistance;
	};
}