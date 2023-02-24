#include "Game.h"
#include "Systems.h"

#include "AssetManager.h"
#include "Map.h"
#include "Constants.h"
#include <iostream>
#include <sstream>

std::unique_ptr<Map> map;
std::shared_ptr<Constants> constants = std::make_shared<Constants>("../data/settings.json");

std::shared_ptr<Manager> Game::manager = std::make_shared<Manager>();
std::unique_ptr<AssetManager> Game::assets = std::make_unique<AssetManager>(manager, constants);

auto collisionSystem(Game::manager->addSystem<CollisionSystem>());
auto playerSystem(Game::manager->addSystem<PlayerSystem>());
auto enemySystem(Game::manager->addSystem<EnemySystem>());
auto projectileSystem(Game::manager->addSystem<ProjectileSystem>());

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::gameEvent;

SDL_Rect Game::camera = { 0, 0,
	constants->MAP_TILE_WIDTH * constants->TILE_SIZE * constants->SCALE - constants->SCREEN_WIDTH,
	constants->MAP_TILE_HEIGHT * constants->TILE_SIZE * constants->SCALE - constants->SCREEN_HEIGHT
};

bool Game::isRunning = false;

auto& label(Game::manager->addEntity());

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

	assets->LoadTextures();
	assets->LoadFonts();

	map = std::make_unique<Map>("terrain", constants->SCALE, constants->TILE_SIZE);
	map->LoadMap("map", constants->MAP_TILE_WIDTH, constants->MAP_TILE_HEIGHT);

	label.addComponent<UILabelComponent>(10, 10, "Test String", "arial", constants->WHITE);

	// todo: remove
	const auto projectileSize = Vector2D(constants->PROJECTILE_SIZE, constants->PROJECTILE_SIZE);
	projectileSystem->instantiateProjectile(Vector2D(500, 600), projectileSize, Vector2D(-2, -2), constants->PROJECTILE_RANGE, 2, "projectile");
	projectileSystem->instantiateProjectile(Vector2D(300, 500), projectileSize, Vector2D(2, -2), constants->PROJECTILE_RANGE, 2, "projectile");
	projectileSystem->instantiateProjectile(Vector2D(400, 400), projectileSize, Vector2D(-2, 0), constants->PROJECTILE_RANGE, 2, "projectile");
	projectileSystem->instantiateProjectile(Vector2D(200, 300), projectileSize, Vector2D(2, 2), constants->PROJECTILE_RANGE, 2, "projectile");
	projectileSystem->instantiateProjectile(Vector2D(600, 200), projectileSize, Vector2D(-2, 2), constants->PROJECTILE_RANGE, 2, "projectile");

	const auto playerPos = Vector2D(constants->SCREEN_WIDTH / 2 - constants->PLAYER_WIDTH - 200, constants->SCREEN_HEIGHT / 2 - constants->PLAYER_HEIGHT);
	playerSystem->instantiatePlayer(playerPos, Vector2D(constants->PLAYER_WIDTH, constants->PLAYER_HEIGHT), constants->SCALE, constants->PLAYER_SPEED, "player");
	enemySystem->instantiateEnemy(Vector2D(playerPos.x - 200, playerPos.y), Vector2D(32, 32), 6.f, 0, "enemy");
}

auto& tiles(Game::manager->getGroup(Game::eGroupLabels::MAP));

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
	manager->refresh();
	manager->update();
	const auto& playerPosition = playerSystem->getPlayerPosition();

	camera.x = static_cast<int>(playerPosition.x - constants->SCREEN_WIDTH / 2);
	camera.y = static_cast<int>(playerPosition.y - constants->SCREEN_HEIGHT / 2);

	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > camera.w) camera.x = camera.w;
	if (camera.y > camera.h) camera.y = camera.h;

	std::stringstream ss;
	ss << "Player position: " << playerPosition;
	label.getComponent<UILabelComponent>().SetLabelText(ss.str(), "arial");
}

void Game::render()
{
	SDL_RenderClear(renderer);
	for (const auto& t : tiles) t->draw();
	manager->draw();

	label.draw();
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