#include "KeyboardManager.h"
#include "ECS.h"
#include "ActorSystem.h"
#include "KeyboardComponent.h"
#include "TransformComponent.h"
#include "DebugParticleComponent.h"
#include "SDL.h"
#include "Constants.h"

#include "CameraManager.h"
#include "ParticleManager.h"

KeyboardManager::KeyboardManager(std::shared_ptr<ecs::Manager> manager) : manager(manager) {}

KeyboardManager::~KeyboardManager() {}

void KeyboardManager::setActorSystem(std::shared_ptr<ecs::ActorSystem> actorSystem)
{
	this->actorSystem = actorSystem;
}

void KeyboardManager::update()
{
	controlledEntities = manager->getEntitiesWithComponent<ecs::KeyboardComponent>();
}

void KeyboardManager::handleEvents(const SDL_Event& gameEvent)
{
	if (actorSystem)
	{
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
		switch (gameEvent.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			Game::isRunning = false;
			pressed = true;
			break;
		case SDLK_p:
			if (!pressed)
			{
				for (const auto& controlledEntity : controlledEntities)
					controlledEntity->getComponent<ecs::DebugParticleComponent>()->play();
				pressed = true;
			}
			break;
		case SDLK_o:
			constants::Constants::Instance().ReloadSettings();
			pressed = true;
			break;
		case SDLK_1:
			if (!pressed)
			{
				for (const auto& controlledEntity : controlledEntities)
					actorSystem->equipWeapon(*controlledEntity, "unarmed");
				pressed = true;
			}
			break;
		case SDLK_2:
			if (!pressed)
			{
				for (const auto& controlledEntity : controlledEntities)
					actorSystem->equipWeapon(*controlledEntity, "shortsword");
				pressed = true;
			}
			break;
		case SDLK_3:
			if (!pressed)
			{
				for (const auto& controlledEntity : controlledEntities)
					actorSystem->equipWeapon(*controlledEntity, "sword");
				pressed = true;
			}
			break;
		case SDLK_4:
			if (!pressed)
			{
				for (const auto& controlledEntity : controlledEntities)
					actorSystem->equipWeapon(*controlledEntity, "bow");
				pressed = true;
			}
			break;
		case SDLK_9:
			for (const auto& controlledEntity : controlledEntities)
			{
				actorSystem->unequipAllArmor(*controlledEntity);
				actorSystem->equipRandomArmor(*controlledEntity);
			}
			break;
		case SDLK_0:
			for (const auto& controlledEntity : controlledEntities)
			{
				actorSystem->unequipAllArmor(*controlledEntity);
			}
			break;
		default:
			break;
		}

		if (!keyboardState[SDL_SCANCODE_ESCAPE] && !keyboardState[SDL_SCANCODE_P] &&
			!keyboardState[SDL_SCANCODE_O] &&
			!keyboardState[SDL_SCANCODE_1] && !keyboardState[SDL_SCANCODE_2] &&
			!keyboardState[SDL_SCANCODE_3] && !keyboardState[SDL_SCANCODE_4] &&
			!keyboardState[SDL_SCANCODE_5])
		{
			pressed = false;
		}
	}

	for (const auto& controlledEntity : controlledEntities)
	{
		const auto keyboardComponent = controlledEntity->getComponent<ecs::KeyboardComponent>();
		keyboardComponent->handleEvents();
	}
}

void KeyboardManager::handleMouse()
{
	SDL_GetMouseState(&mouseX, &mouseY);
}