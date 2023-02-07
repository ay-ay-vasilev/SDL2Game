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
		if (Game::gameEvent.type == SDL_KEYDOWN || Game::gameEvent.type == SDL_KEYUP)
		{
			int x = 0, y = 0;

			switch (Game::gameEvent.key.keysym.sym)
			{
			case SDLK_w:
				y = -1;
				break;
			case SDLK_a:
				x = -1;
				break;
			case SDLK_s:
				y = 1;
				break;
			case SDLK_d:
				x = 1;
				break;
			default:
				break;
			}

			if (Game::gameEvent.type == SDL_KEYDOWN)
			{
				transform->velocity.x += x;
				transform->velocity.y += y;
			}
			else if (Game::gameEvent.type == SDL_KEYUP)
			{
				transform->velocity.x -= x;
				transform->velocity.y -= y;
			}
		}
	}
};