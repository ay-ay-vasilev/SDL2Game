#pragma once
#include "Components.h"

class AIComponentBasicEnemy : public AIComponent, public Observer
{
public:
	AIComponentBasicEnemy() : target(nullptr), distance(0.f), state(eState::IDLE) {}
	virtual ~AIComponentBasicEnemy() {}

	void init() override
	{
		transform = entity->getComponent<TransformComponent>();
		sprite = entity->getComponent<SpriteComponent>();
		sprite->addObserver(this);
		weapon = entity->getComponent<WeaponComponent>();
	}

	void update() override
	{
		if (target)
		{
			distance = Vector2D::Distance(transform->getPosition(), targetTransform->getPosition());

			transform->setVeloctiy(0, 0);
			if (weapon->isInRange(targetHitbox->getHitbox()) && state != eState::ATTACK)
			{
				transform->setDirection(Vector2D::VectorBetween(transform->getPosition(), targetTransform->getPosition()));
				sprite->play("attack");
				state = eState::ATTACK;
			}
			else if (state != eState::ATTACK)
			{
				transform->setDirection(Vector2D::VectorBetween(transform->getPosition(), targetTransform->getPosition()));
				transform->setVeloctiy(Vector2D::VectorBetween(transform->getPosition(), targetTransform->getPosition()));
				sprite->play("walk");
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
		targetHealth->addObserver(this);
		targetTransform = target->getComponent<TransformComponent>();
		targetHitbox = target->getComponent<HitboxComponent>();
	}

	void resetTarget()
	{
		targetHealth.reset();
		targetTransform.reset();
		targetHitbox.reset();
		target = nullptr;

		sprite->play("idle");
		state = eState::IDLE;
		transform->setVeloctiy(0, 0);
		distance = 0.f;

		std::cout << "target lost!\n";
	}

	void loseTarget()
	{
		targetHealth->removeObserver(this);
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
			sprite->play("idle");
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

	std::shared_ptr<SpriteComponent> sprite;
	std::shared_ptr<WeaponComponent> weapon;

	std::shared_ptr<HealthComponent> targetHealth;
	std::shared_ptr<TransformComponent> targetTransform;
	std::shared_ptr<HitboxComponent> targetHitbox;
	std::shared_ptr<TransformComponent> transform;

	eState state;
	float distance;

	// todo refactor to json value
	float aggroDistance = 500;
	float loseAggroDistance = 800;
};