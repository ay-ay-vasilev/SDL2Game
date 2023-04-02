#pragma once
#include "ECS.h"
#include "Observer.h"
#include "AIComponent.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "HitboxComponent.h"
#include "HealthComponent.h"

class AIComponentBasicEnemy : public AIComponent, public Observer
{
public:
	AIComponentBasicEnemy() :
		target(nullptr),
		distance(0.f), aggroDistance(0.f), loseAggroDistance(0.f),
		state(eState::IDLE) {}
	virtual ~AIComponentBasicEnemy() {}

	void init() override
	{
		actorComponent = entity->getComponent<ActorComponent>();
		transform = entity->getComponent<TransformComponent>();
		spriteComponent = entity->getComponent<SpriteComponent>();
		registerWithSubject(spriteComponent);
		weapon = entity->getComponent<WeaponComponent>();
		// todo: read from json
		aggroDistance = 100 * transform->getScale();
		loseAggroDistance = 150 * transform->getScale();
	}

	void update() override
	{
		if (target)
		{
			distance = (Vector2D::Distance(transform->getPosition(), targetTransform->getPosition()));

			transform->setVeloctiy(0, 0);
			auto lockedWeapon = weapon.lock();
			if (!lockedWeapon)
			{
				lockedWeapon = entity->getComponent<WeaponComponent>();
				weapon = lockedWeapon;
			}
			if (!lockedWeapon)
				return;

			if (lockedWeapon->isInRange(targetHitbox->getHitbox()) && state != eState::ATTACK)
			{
				transform->setDirection(Vector2D::VectorBetween(transform->getPosition(), targetTransform->getPosition()));
				actorComponent->playAction("attack");
				state = eState::ATTACK;
			}
			else if (state != eState::ATTACK)
			{
				transform->setDirection(Vector2D::VectorBetween(transform->getPosition(), targetTransform->getPosition()));
				transform->setVeloctiy(Vector2D::VectorBetween(transform->getPosition(), targetTransform->getPosition()));
				actorComponent->playAction("walk");
				state = eState::WALK;
			}
		}
		else
		{
			// default behavior
		}

	}

	void setNewTarget(const Entity* newTarget)
	{
		if (!newTarget || target == newTarget)
			return;
		target = newTarget;
		targetHealth = target->getComponent<HealthComponent>();
		registerWithSubject(targetHealth);
		targetTransform = target->getComponent<TransformComponent>();
		targetHitbox = target->getComponent<HitboxComponent>();
	}

	void resetTarget()
	{
		targetHealth.reset();
		targetTransform.reset();
		targetHitbox.reset();
		target = nullptr;

		actorComponent->playAction("idle");
		state = eState::IDLE;
		transform->setVeloctiy(0, 0);
		distance = 0.f;

		std::cout << "target lost!\n";
	}

	void loseTarget()
	{
		resetTarget();
	}

	void targetDestroyed() { resetTarget(); }
	const float getDistance() const { return distance; }
	const float getAggroDistance() const { return aggroDistance; }
	const float getLoseAggroDistance() const { return loseAggroDistance; }
	const bool hasTarget() const { return target != nullptr; }

	void onNotify(const std::string_view& observedEvent) override
	{
		if (observedEvent == "attack_end")
		{
			actorComponent->playAction("idle");
			state = eState::IDLE;
		}
		if (observedEvent == "player_died")
		{
			targetDestroyed();
		}
	}

private:
	enum class eState
	{
		IDLE,
		WALK,
		ATTACK
	};

	const Entity* target;

	std::shared_ptr<ActorComponent> actorComponent;
	std::shared_ptr<SpriteComponent> spriteComponent;
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