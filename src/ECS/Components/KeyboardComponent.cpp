#include "KeyboardComponent.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "HitboxComponent.h"
#include "ArmorComponent.h"
#include "WeaponComponent.h"
#include "HealthComponent.h"

void ecs::KeyboardComponent::init()
{
	actorComponent = entity->getComponent<ecs::ActorComponent>();
	healthComponent = entity->getComponent<ecs::HealthComponent>();
	registerWithSubject(actorComponent);
	transform = entity->getComponent<ecs::TransformComponent>();
}

void ecs::KeyboardComponent::handleEvents()
{
	if (healthComponent->isDead()) return;

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
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

	if (state != eState::ATTACK && (keyboardState[SDL_GetScancodeFromKey(SDLK_e)] || (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))))
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

	actorComponent->playAction(animName);
	transform->setVeloctiy(static_cast<float>(x), static_cast<float>(y));
}

void ecs::KeyboardComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "attack_end")
	{
		actorComponent->playAction("idle");
		state = eState::IDLE;
	}
}