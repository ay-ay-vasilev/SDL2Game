#pragma once
#include "Components.h"

class AIComponentBasicEnemy : public AIComponent
{
public:
	AIComponentBasicEnemy(Entity& target) : target(target) {}
	virtual ~AIComponentBasicEnemy() {}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		const auto distance = Vector2D::Distance(target.getComponent<TransformComponent>().getPosition(), entity->getComponent<TransformComponent>().getPosition());

		if (distance < 20) // todo REMOVE MAGIC NUM
		{	
			sprite->play("attack");
		}
	}

private:
	Entity& target;
	TransformComponent* transform;
	SpriteComponent* sprite;
};