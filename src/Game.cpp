#include "Game.h"
#include "Systems.h"

#include "AssetManager.h"
#include "Constants.h"
#include <iostream>
#include <sstream>

std::shared_ptr<Constants> Game::constants = std::make_shared<Constants>("../data/settings.json");

std::shared_ptr<Manager> Game::manager = std::make_shared<Manager>(constants);
std::unique_ptr<AssetManager> Game::assets = std::make_unique<AssetManager>(manager);

auto renderSystem(Game::manager->addSystem<RenderSystem>());
auto mapSystem(Game::manager->addSystem<MapSystem>());
auto collisionSystem(Game::manager->addSystem<CollisionSystem>());
auto hitboxWeaponCollisionSystem(Game::manager->addSystem<HitboxWeaponCollisionSystem>());
auto playerSystem(Game::manager->addSystem<PlayerSystem>());
auto enemySystem(Game::manager->addSystem<EnemySystem>());
auto projectileSystem(Game::manager->addSystem<ProjectileSystem>());
auto aiSystem(Game::manager->addSystem<AISystem>());

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::gameEvent;

int Game::cameraMinX = std::min(0, -(constants->SCREEN_WIDTH - (constants->MAP_TILE_WIDTH * constants->TILE_SIZE * static_cast<int>(constants->SCALE))) / 2);
int Game::cameraMinY = std::min(0, -(constants->SCREEN_HEIGHT - (constants->MAP_TILE_HEIGHT * constants->TILE_SIZE * static_cast<int>(constants->SCALE))) / 2);
int Game::cameraMaxX = std::max(cameraMinX, (constants->MAP_TILE_WIDTH * constants->TILE_SIZE * static_cast<int>(constants->SCALE)) - constants->SCREEN_WIDTH);
int Game::cameraMaxY = std::max(cameraMinY, (constants->MAP_TILE_HEIGHT * constants->TILE_SIZE * static_cast<int>(constants->SCALE)) - constants->SCREEN_HEIGHT);

SDL_Rect Game::camera =
{
	0, 0,
	constants->SCREEN_WIDTH,
	constants->SCREEN_HEIGHT
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

		renderer = SDL_CreateRenderer(window, -1, 0);

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
	assets->loadTextures();
	assets->loadFonts();

	label0.addComponent<UILabelComponent>(10, 10, "Test String", "arial", constants->WHITE);

	const std::string projectileFile = "test";
	projectileSystem->instantiateProjectile(Vector2D(83.33, 100), Vector2D(-2, -2), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(50, 83.33), Vector2D(2, -2), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(66.67, 66.67), Vector2D(-2, 0), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(33.33, 50), Vector2D(2, 2), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(100, 33.33), Vector2D(-2, 2), projectileFile);

	playerSystem->instantiatePlayer(constants->PLAYER_POS, "goblin");
	enemySystem->instantiateEnemy(constants->ENEMY_POS, "human");
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
}

void Game::update()
{
	if (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS)
	{
		manager->refresh();
		manager->update();

		const auto& playerPosition = playerSystem->getPlayerPosition();
		
		camera.x = std::clamp(static_cast<int>(playerPosition.x - constants->SCREEN_WIDTH / 2), cameraMinX, cameraMaxX);
		camera.y = std::clamp(static_cast<int>(playerPosition.y - constants->SCREEN_HEIGHT / 2), cameraMinY, cameraMaxY);

		std::stringstream ss0;
		ss0 << "Player position: " << playerPosition;
		label0.getComponent<UILabelComponent>()->SetLabelText(ss0.str(), "arial");
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	manager->draw();

	label0.draw();
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