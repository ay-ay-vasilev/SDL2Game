#include "Game.h"
#include "RenderSystem.h"
#include "MapSystem.h"
#include "CollisionSystem.h"
#include "HitboxWeaponCollisionSystem.h"
#include "PlayerSystem.h"
#include "ActorSystem.h"
#include "ProjectileSystem.h"
#include "AISystem.h"
#include "FactionSystem.h"

#include "UILabelComponent.h"

#include "AssetManager.h"
#include "Constants.h"
#include "KeyboardManager.h"
#include <iostream>
#include <sstream>

std::shared_ptr<Constants> Game::constants = std::make_shared<Constants>("../data/settings.json");

std::shared_ptr<ecs::Manager> Game::manager = std::make_shared<ecs::Manager>(constants);
std::unique_ptr<AssetManager> Game::assets = std::make_unique<AssetManager>(manager);
std::unique_ptr<KeyboardManager> Game::keyboardManager = std::make_unique<KeyboardManager>(manager);

auto renderSystem(Game::manager->addSystem<ecs::RenderSystem>());
auto mapSystem(Game::manager->addSystem<ecs::MapSystem>());
auto collisionSystem(Game::manager->addSystem<ecs::CollisionSystem>());
auto hitboxWeaponCollisionSystem(Game::manager->addSystem<ecs::HitboxWeaponCollisionSystem>());
auto playerSystem(Game::manager->addSystem<ecs::PlayerSystem>());
auto actorSystem(Game::manager->addSystem<ecs::ActorSystem>());
auto projectileSystem(Game::manager->addSystem<ecs::ProjectileSystem>());
auto aiSystem(Game::manager->addSystem<ecs::AISystem>());
auto factionSystem(Game::manager->addSystem<ecs::FactionSystem>());

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::gameEvent;

SDL_Rect Game::camera =
{
	0, 0,
	constants->SCREEN_WIDTH,
	constants->SCREEN_HEIGHT
};

SDL_Rect Game::cameraBounds =
{
	std::min(0, static_cast<int>(-(constants->SCREEN_WIDTH - (constants->MAP_TILE_WIDTH * constants->TILE_SIZE * constants->SCALE)) / 2)),
	std::min(0, static_cast<int>(-(constants->SCREEN_HEIGHT - (constants->MAP_TILE_HEIGHT * constants->TILE_SIZE * constants->SCALE)) / 2)),
	std::max(0, static_cast<int>((constants->MAP_TILE_WIDTH * constants->TILE_SIZE * constants->SCALE) - constants->SCREEN_WIDTH)),
	std::max(0, static_cast<int>((constants->MAP_TILE_HEIGHT * constants->TILE_SIZE * constants->SCALE) - constants->SCREEN_HEIGHT))
};

bool Game::isRunning = false;

auto& label0(Game::manager->addEntity());

Game::Game() : window(nullptr), count(0) {}

Game::~Game() {}

void Game::init()
{
	int flags = 0;
	if (constants->FULLSCREEN)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		std::cout << SDL_GetError() << "\n";
		std::cout << "Subsystem Initialized.\n";

		window = SDL_CreateWindow(
			constants->WINDOW_TITLE.c_str(), 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			constants->SCREEN_WIDTH, constants->SCREEN_HEIGHT,
			flags);

		if (window) std::cout << "Window created!\n";

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, constants->WHITE.r, constants->WHITE.g, constants->WHITE.b, constants->WHITE.a);
			SDL_Event event;
			SDL_PollEvent(&event);
			std::cout << "Renderer created!\n";
		}
		isRunning = true;
	}
	else isRunning = false;

	if (TTF_Init() == -1) std::cout << "Error: SDL_TTF\n";

	manager->setScale(constants->SCALE);
	assets->loadSurfaces();
	assets->loadFonts();

	label0.addComponent<ecs::UILabelComponent>(10, 10, "Test String", "arial", constants->WHITE);

	const std::string projectileFile = "test_projectile";
	projectileSystem->instantiateProjectile(Vector2D(83.33f, 100.f), Vector2D(-2.f, -2.f), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(50.f, 83.33f), Vector2D(2.f, -2.f), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(66.67f, 66.67f), Vector2D(-2.f, 0.f), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(33.33f, 50.f), Vector2D(2.f, 2.f), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(100.f, 33.33f), Vector2D(-2.f, 2.f), projectileFile);

	auto player = playerSystem->instantiatePlayer(constants->PLAYER_POS, "goblin");
	actorSystem->addRandomCustomization(*player);

	for (const auto& humanData : constants->HUMAN_POS)
	{
		auto actor = actorSystem->instantiateActor(humanData, "human");
		actorSystem->equipRandomArmor(*actor);
		actorSystem->equipRandomWeapon(*actor);
		actorSystem->addRandomCustomization(*actor);
	}

	for (const auto& skeletonData : constants->SKELETON_POS)
	{
		auto actor = actorSystem->instantiateActor(skeletonData, "skeleton");
		actorSystem->equipRandomArmor(*actor);
		actorSystem->equipRandomWeapon(*actor);
		actorSystem->addRandomCustomization(*actor);
	}

	for (const auto& goblinData : constants->GOBLIN_POS)
	{
		auto actor = actorSystem->instantiateActor(goblinData, "goblin");
		actorSystem->equipRandomArmor(*actor);
		actorSystem->equipRandomWeapon(*actor);
		actorSystem->addRandomCustomization(*actor);
	}

	mapSystem->instantiateMap("terrain", constants->TILE_SIZE, "map", constants->MAP_TILE_WIDTH, constants->MAP_TILE_HEIGHT);
}

void Game::handleEvents()
{
	SDL_PollEvent(&gameEvent);
	switch (gameEvent.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}

	keyboardManager->handleEvents();
}

void Game::update(double delta)
{
	if (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS)
	{
		manager->refresh();
		manager->update(delta);
		keyboardManager->update();

		const auto& playerPosition = playerSystem->getPlayerPosition();
		
		camera.x = std::clamp(static_cast<int>(playerPosition.x - constants->SCREEN_WIDTH / 2), cameraBounds.x, cameraBounds.x + cameraBounds.w);
		camera.y = std::clamp(static_cast<int>(playerPosition.y - constants->SCREEN_HEIGHT / 2), cameraBounds.y, cameraBounds.y + cameraBounds.h);

		std::stringstream ss0;
		ss0 << "Player position: " << playerPosition;
		label0.getComponent<ecs::UILabelComponent>()->SetLabelText(ss0.str(), "arial");
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	manager->draw();

	//label0.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned!\n";
}

bool Game::running()
{
	return isRunning;
}