#pragma once

#include "Components.h"
#include "Observer.h"

class KeyboardController : public Component, private Observer
{
public:
	virtual ~KeyboardController()
	{
		sprite->removeObserver(this);
	}

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
	}

	void update() override
	{
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

		switch (Game::gameEvent.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			Game::isRunning = false;
			break;
		case SDLK_o:
			Game::constants->ReloadSettings();
			break;
		}

		int x = 0, y = 0;
		std::string animName = "idle";

		if (state != eState::ATTACK)
		{
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_w)] || keyboardState[SDL_GetScancodeFromKey(SDLK_UP)])
			{
				y -= 1;
				transform->setDirectionY(-1.f);
				animName = "walk";
			}
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_a)] || keyboardState[SDL_GetScancodeFromKey(SDLK_LEFT)])
			{
				x -= 1;
				transform->setDirectionX(-1.f);
				animName = "walk";
			}
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_s)] || keyboardState[SDL_GetScancodeFromKey(SDLK_DOWN)])
			{
				y += 1;
				transform->setDirectionY(1.f);
				animName = "walk";
			}
			if (keyboardState[SDL_GetScancodeFromKey(SDLK_d)] || keyboardState[SDL_GetScancodeFromKey(SDLK_RIGHT)])
			{
				x += 1;
				transform->setDirectionX(1.f);
				animName = "walk";
			}
		}

		if (state != eState::ATTACK && (keyboardState[SDL_GetScancodeFromKey(SDLK_e)] || (SDL_GetMouseState(NULL, NULL)& SDL_BUTTON(SDL_BUTTON_LEFT))))
		{
			animName = "attack";
			x = 0; y = 0;
		}

		if (animName == "idle")
		{
			if (state == eState::ATTACK) animName = "attack";
			else state = eState::IDLE;
		}
		else if (animName == "walk") state = eState::WALK;
		else if (animName == "attack") state = eState::ATTACK;

		sprite->play(animName);
		transform->setVeloctiy(static_cast<float>(x), static_cast<float>(y));
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
	eState state = eState::IDLE;
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<SpriteComponent> sprite;
};