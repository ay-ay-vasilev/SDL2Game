#pragma once
#include "Components.h"

class AIComponentBasicEnemy : public AIComponent, public Observer
{
public:
	AIComponentBasicEnemy(const Entity* target) : target(target) {}
	virtual ~AIComponentBasicEnemy() {}

	enum class eState
	{
		IDLE,
		WALK,
		ATTACK
	};

	void init() override
	{
		transform = entity->getComponent<TransformComponent>();

		sprite = entity->getComponent<SpriteComponent>();
		sprite->addObserver(this);

		targetHealth = target->getComponent<HealthComponent>();
		targetHealth->addObserver(this);

		targetTransform = target->getComponent<TransformComponent>();
	}

	void update() override
	{
		if (target)
		{
			distance = Vector2D::Distance(targetTransform->getPosition(), transform->getPosition());
			if (distance < 32 * 6 && state != eState::ATTACK) // todo REMOVE MAGIC NUM + ADD STATES FROM KEYBOARD CONTROLLER
			{
				transform->setDirection(Vector2D::VectorBetween(transform->getPosition(), targetTransform->getPosition()));
				sprite->play("attack");
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
	const Entity* target; // redo with smart pointer!
	std::shared_ptr<HealthComponent> targetHealth;
	std::shared_ptr<TransformComponent> targetTransform;
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<SpriteComponent> sprite;
	eState state = eState::IDLE;
	int distance;
};