#include "KeyboardComponent.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "HitboxComponent.h"
#include "ArmorComponent.h"
#include "WeaponComponent.h"

void ecs::KeyboardComponent::init()
{
	actorComponent = entity->getComponent<ecs::ActorComponent>();
	registerWithSubject(actorComponent);
	transform = entity->getComponent<ecs::TransformComponent>();
}

void ecs::KeyboardComponent::update()
{
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	switch (Game::gameEvent.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		Game::isRunning = false;
		pressed = true;
		break;
	case SDLK_o:
		Game::constants->ReloadSettings();
		pressed = true;
		break;
	case SDLK_1:
		if (!pressed)
		{
			equipWeapon("unarmed");
			pressed = true;
		}
		break;
	case SDLK_2:
		if (!pressed)
		{
			equipWeapon("shortsword");
			pressed = true;
		}
		break;
	case SDLK_3:
		if (!pressed)
		{
			equipWeapon("sword");
			pressed = true;
		}
		break;
	case SDLK_4:
		if (!pressed)
		{
			equipArmor();
			pressed = true;
		}
		break;
	case SDLK_5:
		if (!pressed)
		{
			unequipArmor();
			pressed = true;
		}
		break;
	default:
		break;
	}

	if (!keyboardState[SDL_SCANCODE_ESCAPE] && !keyboardState[SDL_SCANCODE_O] &&
		!keyboardState[SDL_SCANCODE_1] && !keyboardState[SDL_SCANCODE_2] &&
		!keyboardState[SDL_SCANCODE_3] && !keyboardState[SDL_SCANCODE_4] &&
		!keyboardState[SDL_SCANCODE_5])
	{
		pressed = false;
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

void ecs::KeyboardComponent::equipWeapon(const std::string& weaponName)
{
	if (!entity->hasComponent<ecs::ActorComponent>()) return;

	const std::string entityType = entity->getComponent<ecs::ActorComponent>()->getActorType();

	if (entity->hasComponent<ecs::WeaponComponent>())
	{
		if (entity->getComponent<ecs::WeaponComponent>()->getTag() == weaponName)
			return;
		entity->removeComponent<ecs::WeaponComponent>();
	}
	entity->addComponent<ecs::WeaponComponent>(weaponName, entityType);
}

void ecs::KeyboardComponent::equipArmor()
{
	if (!entity->hasComponent<ecs::ActorComponent>()) return;

	const std::string entityType = entity->getComponent<ecs::ActorComponent>()->getActorType();

	if (entity->hasComponent<ecs::ArmorComponent>())
	{
		const auto armorComponent = entity->getComponent<ecs::ArmorComponent>();
		//armorComponent->equipArmorToSlot("pants_light", playerTag, "pants");
		//armorComponent->equipArmorToSlot("long_sleeve_light", playerTag, "shirt");
		//armorComponent->equipArmorToSlot("light_jacket_brown", playerTag, "jacket");

		armorComponent->equipArmorToSlot("pants_brown", entityType, "pants");
		armorComponent->equipArmorToSlot("shirt_light", entityType, "shirt");
		armorComponent->equipArmorToSlot("jacket_dark", entityType, "jacket");

		armorComponent->equipArmorToSlot("eyepatch_black", entityType, "eyewear");
		armorComponent->equipArmorToSlot("scarf_black", entityType, "mask");
		//armorComponent->equipArmorToSlot("cloak_0", playerTag, "overcoat");
	}
}

void ecs::KeyboardComponent::unequipArmor()
{
	if (entity->hasComponent<ecs::ArmorComponent>())
	{
		const auto armorComponent = entity->getComponent<ecs::ArmorComponent>();
		armorComponent->unequipAllArmor();
	}
}