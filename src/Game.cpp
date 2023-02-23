#include "Game.h"
#include "TextureManager.h"
#include "EnemyManager.h"
#include "AssetManager.h"
#include "Components.h"
#include "Map.h"
#include "Constants.h"
#include <iostream>
#include <sstream>

Map* map;
Constants constants {"../data/settings.json"};

std::unique_ptr<Manager> Game::manager = std::make_unique<Manager>();
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::gameEvent;

SDL_Rect Game::camera = { 0, 0,
	constants.MAP_TILE_WIDTH * constants.TILE_SIZE * constants.SCALE - constants.SCREEN_WIDTH,
	constants.MAP_TILE_HEIGHT * constants.TILE_SIZE * constants.SCALE - constants.SCREEN_HEIGHT
};

std::unique_ptr<AssetManager> Game::assets = std::make_unique<AssetManager>(manager.get());
std::unique_ptr<EnemyManager> Game::enemyManager = std::make_unique<EnemyManager>(manager.get());

bool Game::isRunning = false;

auto& player(Game::manager->addEntity());
auto& label(Game::manager->addEntity());
auto collisionSystem(Game::manager->addSystem<CollisionSystem>());

Game::Game() : window(nullptr), count(0)
{
}

Game::~Game()
{
}

void Game::init()
{
	int flags = 0;
	if (constants.FULLSCREEN)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		std::cout << SDL_GetError() << "\n";
		std::cout << "Subsystem Initialized.\n";

		window = SDL_CreateWindow(
			constants.WINDOW_TITLE.c_str(), 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			constants.SCREEN_WIDTH, constants.SCREEN_HEIGHT,
			flags);

		if (window)
		{
			std::cout << "Window created!\n";
		}

		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, constants.WHITE.r, constants.WHITE.g, constants.WHITE.b, constants.WHITE.a);
			SDL_Event event;
			SDL_PollEvent(&event);
			std::cout << "Renderer created!\n";
		}

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error: SDL_TTF\n";
	}

	assets->AddTexture("terrain", "assets/images/sprite_sheets/tiles_v0.png");
	assets->AddTexture("player", "assets/images/sprite_sheets/goblin_downscale_spritesheet.png");
	assets->AddTexture("projectile", "assets/images/test_projectile.png");
	assets->AddTexture("enemy", "assets/images/sprite_sheets/human_downscale_spritesheet.png");

	assets->AddFont("arial", "../assets/fonts/arial.ttf", constants.DEBUG_FONT_SIZE);

	map = new Map("terrain", constants.SCALE, constants.TILE_SIZE);

	map->LoadMap("map", constants.MAP_TILE_WIDTH, constants.MAP_TILE_HEIGHT);

	auto playerPos = Vector2D(constants.SCREEN_WIDTH / 2 - constants.PLAYER_WIDTH - 200, constants.SCREEN_HEIGHT / 2 - constants.PLAYER_HEIGHT);
	player.addComponent<TransformComponent>(playerPos.x, playerPos.y, constants.PLAYER_WIDTH, constants.PLAYER_HEIGHT, constants.SCALE, constants.PLAYER_SPEED);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(eGroupLabels::PLAYERS);

	label.addComponent<UILabelComponent>(10, 10, "Test String", "arial", constants.WHITE);

	// todo: remove
	const auto projectileSize = Vector2D(constants.PROJECTILE_SIZE, constants.PROJECTILE_SIZE);

	assets->CreateProjectile(Vector2D(500, 600), projectileSize, Vector2D(-2, -2), constants.PROJECTILE_RANGE, 2, "projectile");
	assets->CreateProjectile(Vector2D(300, 500), projectileSize, Vector2D(2, -2), constants.PROJECTILE_RANGE, 2, "projectile");
	assets->CreateProjectile(Vector2D(400, 400), projectileSize, Vector2D(-2, 0), constants.PROJECTILE_RANGE, 2, "projectile");
	assets->CreateProjectile(Vector2D(200, 300), projectileSize, Vector2D(2, 2), constants.PROJECTILE_RANGE, 2, "projectile");
	assets->CreateProjectile(Vector2D(600, 200), projectileSize, Vector2D(-2, 2), constants.PROJECTILE_RANGE, 2, "projectile");

	enemyManager->InstantiateEnemy(Vector2D(playerPos.x - 200, playerPos.y), Vector2D(32, 32), 6.f, 0, "enemy");
}

auto& tiles(Game::manager->getGroup(Game::eGroupLabels::MAP));
auto& projectiles(Game::manager->getGroup(Game::eGroupLabels::PROJECTILES));
auto& players(Game::manager->getGroup(Game::eGroupLabels::PLAYERS));
auto& enemies(Game::manager->getGroup(Game::eGroupLabels::ENEMIES));

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

	const auto& playerPosition = player.getComponent<TransformComponent>().position;

	camera.x = static_cast<int>(playerPosition.x - constants.SCREEN_WIDTH / 2);
	camera.y = static_cast<int>(playerPosition.y - constants.SCREEN_HEIGHT / 2);

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

	std::stringstream ss;
	ss << "Player position: " << playerPosition;
	label.getComponent<UILabelComponent>().SetLabelText(ss.str(), "arial");
}

void Game::render()
{
	SDL_RenderClear(renderer);
	for (const auto& t : tiles) t->draw();
	for (const auto& p : projectiles) p->draw();
	for (const auto& p : players) p->draw();
	for (const auto& e : enemies) e->draw();
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