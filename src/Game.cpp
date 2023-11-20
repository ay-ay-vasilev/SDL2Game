#include "Game.h"
#include "RenderSystem.h"
#include "MapSystem.h"
#include "CollisionSystem.h"
#include "HitboxWeaponCollisionSystem.h"
#include "ActorSystem.h"
#include "ProjectileSystem.h"
#include "AISystem.h"
#include "FactionSystem.h"
#include "SplatterSystem.h"

#include "UILabelComponent.h"
#include "KeyboardComponent.h"
#include "TransformComponent.h"

#include "AssetManager.h"
#include "Constants.h"
#include "KeyboardManager.h"
#include "CameraManager.h"
#include "ParticleManager.h"

#include <iostream>
#include <sstream>

#include <entt/entt.hpp>

// Singletons
std::shared_ptr<ecs::Manager> Game::manager = std::make_shared<ecs::Manager>();
std::unique_ptr<assets::AssetManager> Game::assetManager = std::make_unique<assets::AssetManager>(manager);
std::unique_ptr<KeyboardManager> Game::keyboardManager = std::make_unique<KeyboardManager>(manager);
std::unique_ptr<CameraManager> Game::cameraManager = std::make_unique<CameraManager>(manager);
std::unique_ptr<ParticleManager> Game::particleManager = std::make_unique<ParticleManager>();

SDL_Renderer* Game::renderer = nullptr;
bool Game::isRunning = false;

entt::registry registry;

// test - remove later
ecs::Entity& label0 = Game::manager->addEntity();

Game::Game() : window(nullptr), gameEvent(), count(0) {}

Game::~Game() {}

void Game::init()
{
	int flags = 0;
	auto& constants = constants::Constants::Instance();
	constants.Init("../data/settings.json");

	const auto& FULLSCREEN = constants::GetConstant<bool>(constants, "fullscreen");
	const auto& SCALE = constants::GetConstant<float>(constants, "scale");
	const auto& WINDOW_TITLE = constants::GetConstant<std::string>(constants, "window_title").c_str();
	const auto& SCREEN_WIDTH = constants::GetConstant<int>(constants, "screen_width");
	const auto& SCREEN_HEIGHT = constants::GetConstant<int>(constants, "screen_height");
	const auto& COLOR_WHITE = constants::GetConstant<SDL_Color>(constants, "color_white");
	const auto& PLAYER_POS = constants::GetConstant<Vector2D>(constants, "player_pos");
	const auto& PLAYER_RACE = constants::GetConstant<std::string>(constants, "player_race");
	const auto& DEBUG_PROJECTILES = constants::GetConstant<std::vector<constants::ProjectileData>>(constants, "debug_projectiles");
	const auto& HUMAN_POSITIONS = constants::GetConstant<std::vector<Vector2D>>(constants, "human_positions");
	const auto& SKELETON_POSITIONS = constants::GetConstant<std::vector<Vector2D>>(constants, "skeleton_positions");
	const auto& GOBLIN_POSITIONS = constants::GetConstant<std::vector<Vector2D>>(constants, "goblin_positions");
	const auto& MAP_TILE_WIDTH = constants::GetConstant<int>(constants, "map_tile_width");
	const auto& MAP_TILE_HEIGHT = constants::GetConstant<int>(constants, "map_tile_height");
	const auto& TILE_SIZE = constants::GetConstant<int>(constants, "tile_size");

	if (FULLSCREEN)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		std::cout << SDL_GetError() << "\n";
		std::cout << "Subsystem Initialized.\n";

		window = SDL_CreateWindow(
			WINDOW_TITLE,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			flags);

		if (window) std::cout << "Window created!\n";

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, COLOR_WHITE.a);
			SDL_Event event;
			SDL_PollEvent(&event);
			std::cout << "Renderer created!\n";
		}
		isRunning = true;
	}
	else isRunning = false;

	if (TTF_Init() == -1) std::cout << "Error: SDL_TTF\n";

	manager->setScale(SCALE);
	assetManager->loadSurfaces();
	assetManager->loadFonts();

	// Systems
	auto actorSystem(Game::manager->addSystem<ecs::ActorSystem>());
	auto projectileSystem(Game::manager->addSystem<ecs::ProjectileSystem>());
	auto mapSystem(Game::manager->addSystem<ecs::MapSystem>());
	Game::manager->addSystem<ecs::RenderSystem>();
	Game::manager->addSystem<ecs::CollisionSystem>();
	Game::manager->addSystem<ecs::HitboxWeaponCollisionSystem>();
	Game::manager->addSystem<ecs::AISystem>();
	Game::manager->addSystem<ecs::FactionSystem>();
	Game::manager->addSystem<ecs::SplatterSystem>();

	keyboardManager->setActorSystem(actorSystem);
	cameraManager->init();
	particleManager->init();

	label0.addComponent<ecs::UILabelComponent>(10, 10, "Test String", "arial", COLOR_WHITE);

	auto player = actorSystem->instantiatePlayer(PLAYER_POS, PLAYER_RACE);
	actorSystem->addRandomCustomization(*player);
	actorSystem->equipRandomArmor(*player);

	const std::string projectileFile = "test_projectile";
	for (const auto& projectileData : DEBUG_PROJECTILES)
	{
		projectileSystem->instantiateProjectile
		(
			player->getID(),
			{ projectileData.pos.x * SCALE, projectileData.pos.y * SCALE },
			projectileData.velocity,
			projectileFile
		);
	}

	for (const auto& humanData : HUMAN_POSITIONS)
	{
		auto actor = actorSystem->instantiateActor(humanData, "human");
		actorSystem->addRandomCustomization(*actor);
		actorSystem->equipRandomArmor(*actor);
		actorSystem->equipRandomWeapon(*actor);
	}

	for (const auto& skeletonData : SKELETON_POSITIONS)
	{
		auto actor = actorSystem->instantiateActor(skeletonData, "skeleton");
		actorSystem->addRandomCustomization(*actor);
		actorSystem->equipRandomArmor(*actor);
		actorSystem->equipRandomWeapon(*actor);
	}

	for (const auto& goblinData : GOBLIN_POSITIONS)
	{
		auto actor = actorSystem->instantiateActor(goblinData, "goblin");
		actorSystem->addRandomCustomization(*actor);
		actorSystem->equipRandomArmor(*actor);
		actorSystem->equipRandomWeapon(*actor);
	}

	mapSystem->instantiateMap("terrain", TILE_SIZE, "map", MAP_TILE_WIDTH, MAP_TILE_HEIGHT);
}

void Game::handleEvents()
{
	while (SDL_PollEvent(&gameEvent) != 0)
	{
		switch (gameEvent.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			keyboardManager->handleEvents(gameEvent);
			break;
		default:
			break;
		}
	}
	keyboardManager->handleMouse();
}

void Game::update(double delta)
{
	if (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS)
	{
		manager->refresh();
		manager->update(delta);
		particleManager->update();
		keyboardManager->update();
		cameraManager->update();

		const auto& players = manager->getEntitiesWithComponent<ecs::KeyboardComponent>();
		const Vector2D& playerPosition = players[0]->getComponent<ecs::TransformComponent>()->getPosition();

		std::stringstream ss0;
		ss0 << "Player position: " << playerPosition;
		label0.getComponent<ecs::UILabelComponent>()->SetLabelText(ss0.str(), "arial");
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	manager->draw();
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