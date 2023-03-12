#pragma once
#include "Components.h"

class AIComponentBasicEnemy : public AIComponent, public Observer
{
public:
	AIComponentBasicEnemy(const Entity* target) : target(target) {}
	virtual ~AIComponentBasicEnemy() {}

	void init() override
	{
		transform = entity->getComponent<TransformComponent>();
		sprite = entity->getComponent<SpriteComponent>();
		sprite->addObserver(this);
		weapon = entity->getComponent<WeaponComponent>();

		targetHealth = target->getComponent<HealthComponent>();
		targetHealth->addObserver(this);

		targetTransform = target->getComponent<TransformComponent>();
		targetHitbox = target->getComponent<HitboxComponent>();
	}

	void update() override
	{
		if (target)
		{
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
			// switch to default behavior

		}

	}

	void onNotify(const std::string_view& observedEvent) override
	{
		if (observedEvent == "attack_end")
		{
			sprite->play("idle");
			state = eState::IDLE;
		}
		if (observedEvent == "player_died")
		{
			target = nullptr;
			std::cout << "target lost!\n";
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

	eState state = eState::IDLE;
	float distance;
};