#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include "Observer.h"

class KeyboardController : public Component, private Observer
{
public:
	enum class eState
	{
		IDLE,
		WALK,
		ATTACK
	};

	eState state = eState::IDLE;
	TransformComponent* transform;
	SpriteComponent* sprite;
	
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		sprite->addObserver(this);
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

		std::string animName = "idle";

		if (state != eState::ATTACK)
		{
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_w)])
			{
				y -= 1;
				animName = "walk";
			}
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_a)])
			{
				x -= 1;
				animName = "walk";
				sprite->spriteFlip = SDL_FLIP_NONE;
			}
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_s)])
			{
				y += 1;
				animName = "walk";
			}
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_d)])
			{
				x += 1;
				animName = "walk";
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			}
		}

		if (state != eState::ATTACK && (keyboardState[SDL_GetScancodeFromKey(SDLK_e)] || (SDL_GetMouseState(NULL, NULL)& SDL_BUTTON(SDL_BUTTON_LEFT))))
		{
			animName = "attack";
			x = 0; y = 0;
		}

		if (animName == "idle")
		{
			if (state == eState::ATTACK)
			{
				animName = "attack";
			}
			else
			{
				state = eState::IDLE;
			}
		}
		else if (animName == "walk")
		{
			state = eState::WALK;
		}
		else if (animName == "attack")
			state = eState::ATTACK;


		sprite->play(animName);

		transform->velocity.x = static_cast<float>(x);
		transform->velocity.y = static_cast<float>(y);
	}

	void onNotify(const std::string_view& observedEvent) override
	{
		if (observedEvent == "attack_end")
		{
			sprite->play("idle");
			state = eState::IDLE;
		}
	}

};