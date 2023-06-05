#include "KeyboardManager.h"
#include "ECS.h"
#include "ActorSystem.h"
#include "KeyboardComponent.h"
#include "TransformComponent.h"
#include "SDL.h"
#include "Game.h"

#include "CameraManager.h"

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

void KeyboardManager::handleEvents()
{
	if (actorSystem)
	{
		auto playerPosition = manager->getGroup(Game::eGroupLabels::PLAYERS).front()->getComponent<ecs::TransformComponent>()->getPosition();
		const auto playerDirection = manager->getGroup(Game::eGroupLabels::PLAYERS).front()->getComponent<ecs::TransformComponent>()->getDirection();
		const auto playerVelocity = manager->getGroup(Game::eGroupLabels::PLAYERS).front()->getComponent<ecs::TransformComponent>()->getVelocity();
		const auto playerSpeed = manager->getGroup(Game::eGroupLabels::PLAYERS).front()->getComponent<ecs::TransformComponent>()->getSpeed();

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
			for (const auto& controlledEntity : controlledEntities)
			{
				actorSystem->unequipAllArmor(*controlledEntity);
				actorSystem->equipRandomArmor(*controlledEntity);
			}
			break;
		case SDLK_5:
			for (const auto& controlledEntity : controlledEntities)
			{
				actorSystem->unequipAllArmor(*controlledEntity);
			}
			break;
		default:
			break;
		}

		if (!keyboardState[SDL_SCANCODE_ESCAPE] && !keyboardState[SDL_SCANCODE_TAB] &&
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