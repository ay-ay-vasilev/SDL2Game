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
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		sprite->addObserver(this);
	}

	void update() override
	{
		if (target)
			distance = Vector2D::Distance(target->getComponent<TransformComponent>().getPosition(), entity->getComponent<TransformComponent>().getPosition());

		if (distance < 32 * 6 && state != eState::ATTACK) // todo REMOVE MAGIC NUM + ADD STATES FROM KEYBOARD CONTROLLER
		{
			sprite->play("attack");
		}
	}

	void onNotify(const std::string_view& observedEvent) override
	{
		if (observedEvent == "attack_end")
		{
			sprite->play("idle");
			state = eState::IDLE;
		}
	}

private:
	const Entity* target; // redo with smart pointer!
	TransformComponent* transform;
	SpriteComponent* sprite;
	eState state = eState::IDLE;
	int distance;
};