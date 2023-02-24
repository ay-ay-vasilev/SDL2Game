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

		switch (Game::gameEvent.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			Game::isRunning = false;
			break;
		}

		int x = 0, y = 0;

		sprite->Play("idle");
		if (keyboardState[SDL_GetScancodeFromKey(SDLK_w)])
		{
			y -= 1;
			sprite->Play("walk");
		}
		if (keyboardState[SDL_GetScancodeFromKey(SDLK_a)])
		{
			x -= 1;
			sprite->Play("walk");
			sprite->spriteFlip = SDL_FLIP_NONE;
		}
		if (keyboardState[SDL_GetScancodeFromKey(SDLK_s)])
		{
			y += 1;
			sprite->Play("walk");
		}
		if (keyboardState[SDL_GetScancodeFromKey(SDLK_d)])
		{
			x += 1;
			sprite->Play("walk");
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}

		transform->velocity.x = static_cast<float>(x);
		transform->velocity.y = static_cast<float>(y);
	}
};