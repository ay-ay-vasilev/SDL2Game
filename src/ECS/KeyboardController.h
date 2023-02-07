#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

		int x = 0, y = 0;

		if (keyboardState[SDL_GetScancodeFromKey(SDLK_w)]) y -= 1;
		if (keyboardState[SDL_GetScancodeFromKey(SDLK_a)]) x -= 1;
		if (keyboardState[SDL_GetScancodeFromKey(SDLK_s)]) y += 1;
		if (keyboardState[SDL_GetScancodeFromKey(SDLK_d)]) x += 1;

		transform->velocity.x = x;
		transform->velocity.y = y;
	}
};