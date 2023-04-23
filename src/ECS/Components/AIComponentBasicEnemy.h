#pragma once
#include "ECS.h"
#include "Observer.h"
#include "AIComponent.h"

class ActorComponent;
class TransformComponent;
class HitboxComponent;
class HealthComponent;
class WeaponComponent;
class AIComponentBasicEnemy : public AIComponent, public Observer
{
public:
	AIComponentBasicEnemy();
	virtual ~AIComponentBasicEnemy() {}

	// Component
	void init() override;
	void update() override;

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
	std::string targetTag;

	std::shared_ptr<ActorComponent> actorComponent;
	std::weak_ptr<WeaponComponent> weapon;

	std::shared_ptr<HealthComponent> targetHealth;
	std::shared_ptr<TransformComponent> targetTransform;
	std::shared_ptr<HitboxComponent> targetHitbox;
	std::shared_ptr<TransformComponent> transform;

	eState state;
	float distance;
	float aggroDistance;
	float loseAggroDistance;
};