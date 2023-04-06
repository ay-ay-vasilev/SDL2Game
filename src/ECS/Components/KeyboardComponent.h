#pragma once
#include "ECS.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "HitboxComponent.h"
#include "ArmorComponent.h"
#include "Observer.h"

class KeyboardComponent : public Component, private Observer
{
public:
	virtual ~KeyboardComponent() {}

	enum class eState
	{
		IDLE,
		WALK,
		ATTACK
	};

	void init() override
	{
		actorComponent = entity->getComponent<ActorComponent>();
		registerWithSubject(actorComponent);
		transform = entity->getComponent<TransformComponent>();
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
		case SDLK_1:
			equipWeapon("unarmed");
			break;
		case SDLK_2:
			equipWeapon("shortsword");
			break;
		case SDLK_3:
			equipWeapon("sword");
			break;
		case SDLK_4:
			equipArmor();
			break;
		case SDLK_5:
			unequipArmor();
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

		actorComponent->playAction(animName);
		transform->setVeloctiy(static_cast<float>(x), static_cast<float>(y));
	}

	void onNotify(const std::string_view& observedEvent) override
	{
		if (observedEvent == "attack_end")
		{
			actorComponent->playAction("idle");
			state = eState::IDLE;
		}
	}

private:
	void equipWeapon(const std::string& weaponName)
	{
		std::string entityTag = "";
		if (entity->hasComponent<HitboxComponent>())
		{
			const auto hitboxComponent = entity->getComponent<HitboxComponent>();
			entityTag = hitboxComponent->getTag();
		}
		if (entity->hasComponent<WeaponComponent>())
		{
			if (entity->getComponent<WeaponComponent>()->getTag() == weaponName)
				return;
			entity->removeComponent<WeaponComponent>();
		}
		entity->addComponent<WeaponComponent>(weaponName, entityTag);
	}

	void equipArmor()
	{
		std::string playerTag = "";
		if (entity->hasComponent<HitboxComponent>())
		{
			const auto hitboxComponent = entity->getComponent<HitboxComponent>();
			playerTag = hitboxComponent->getTag();
		}
		if (entity->hasComponent<ArmorComponent>())
		{
			const auto armorComponent = entity->getComponent<ArmorComponent>();
			armorComponent->equipArmorToSlot("0", playerTag, "pants");
			armorComponent->equipArmorToSlot("0", playerTag, "shirt");
			armorComponent->equipArmorToSlot("0", playerTag, "jacket");
		}
	}

	void unequipArmor()
	{
		if (entity->hasComponent<ArmorComponent>())
		{
			const auto armorComponent = entity->getComponent<ArmorComponent>();
			armorComponent->unequipArmorFromSlot("pants");
			armorComponent->unequipArmorFromSlot("shirt");
			armorComponent->unequipArmorFromSlot("jacket");
		}
	}

	eState state = eState::IDLE;
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<ActorComponent> actorComponent;
};