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

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::gameEvent;

SDL_Rect Game::camera = { 0, 0,
	constants->MAP_TILE_WIDTH * constants->TILE_SIZE * static_cast<int>(constants->SCALE) - constants->SCREEN_WIDTH,
	constants->MAP_TILE_HEIGHT * constants->TILE_SIZE * static_cast<int>(constants->SCALE) - constants->SCREEN_HEIGHT
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

	manager->setScale(constants->SCALE);
	assets->loadTextures();
	assets->loadFonts();


	label.addComponent<UILabelComponent>(10, 10, "Test String", "arial", constants->WHITE);

	const std::string projectileFile = "test";
	projectileSystem->instantiateProjectile(Vector2D(500, 600), Vector2D(-2, -2), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(300, 500), Vector2D(2, -2), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(400, 400), Vector2D(-2, 0), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(200, 300), Vector2D(2, 2), projectileFile);
	projectileSystem->instantiateProjectile(Vector2D(600, 200), Vector2D(-2, 2), projectileFile);

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
	label.getComponent<UILabelComponent>()->SetLabelText(ss.str(), "arial");
}

void Game::render()
{
	SDL_RenderClear(renderer);
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