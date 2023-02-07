#pragma once
#include <string>
#include <SDL.h>
#include "Components.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent(std::string tag) : tag(tag) {};

	SDL_Rect collider;
	std::string tag;

	TransformComponent* transform;

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		collider.x = (int)(transform->position.x);
		collider.y = (int)(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}
};