#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;
	
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

		sprite->spriteFlip = SDL_FLIP_NONE;
		int x = 0, y = 0;

		if (keyboardState[SDL_GetScancodeFromKey(SDLK_w)])
		{
			y -= 1;
			sprite->Play("idle");
		}
		if (keyboardState[SDL_GetScancodeFromKey(SDLK_a)])
		{
			x -= 1;
			sprite->Play("idle");
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		if (keyboardState[SDL_GetScancodeFromKey(SDLK_s)])
		{
			y += 1;
			sprite->Play("idle");
		}
		if (keyboardState[SDL_GetScancodeFromKey(SDLK_d)])
		{
			x += 1;
			sprite->Play("idle");
		}

		transform->velocity.x = x;
		transform->velocity.y = y;
	}
};