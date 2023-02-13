#include "Game.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Map.h"
#include "Components.h"
#include "Collision.h"
#include <iostream>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::gameEvent;
SDL_Rect Game::camera = { 0, 0, 16 * 32 * 6 - 800, 16 * 32 * 6 - 640};

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());

Game::Game() : window(nullptr), count(0)
{
}

Game::~Game()
{
}

void Game::init(std::string title, int x, int y, int width, int height, bool fullScreen)
{
	int flags = 0;
	if (fullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		std::cout << SDL_GetError() << "\n";
		std::cout << "Subsystem Initialized.\n";

		window = SDL_CreateWindow(title.data(), x, y, width, height, flags);

		if (window)
		{
			std::cout << "Window created!\n";
		}

		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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

	assets->AddTexture("terrain", "art/tiles_v0.png");
	assets->AddTexture("player", "art/goblin_downscale_spritesheet.png");
	assets->AddTexture("projectile", "art/test_projectile.png");

	map = new Map("terrain", 6, 32);

	map->LoadMap("map", 16, 16);

	player.addComponent<TransformComponent>(6.f);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(eGroupLabels::PLAYERS);

	assets->CreateProjectile(Vector2D(500, 600), Vector2D(-2, -2), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(300, 500), Vector2D(2, -2), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(400, 400), Vector2D(-2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(200, 300), Vector2D(2, 2), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(600, 200), Vector2D(-2, 2), 200, 2, "projectile");
}

auto& tiles(manager.getGroup(Game::eGroupLabels::MAP));
auto& players(manager.getGroup(Game::eGroupLabels::PLAYERS));
auto& enemies(manager.getGroup(Game::eGroupLabels::ENEMIES));
auto& colliders(manager.getGroup(Game::eGroupLabels::COLLIDERS));
auto& projectiles(manager.getGroup(Game::eGroupLabels::PROJECTILES));

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
	SDL_Rect playerCollider = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPosition = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	for (const auto& c : colliders)
	{
		SDL_Rect collider = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(collider, playerCollider))
		{
			player.getComponent<TransformComponent>().position = playerPosition;
		}
	}

	for (const auto& projectile : projectiles)
	{
		SDL_Rect collider = projectile->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(collider, playerCollider))
		{
			projectile->destroy();
			std::cout << "projectile: hit the player!\n";
		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - 400;
	camera.y = player.getComponent<TransformComponent>().position.y - 320;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
}

void Game::render()
{
	SDL_RenderClear(renderer);
	for (const auto& t : tiles) t->draw();
	for (const auto& c : colliders) c->draw();
	for (const auto& p : projectiles) p->draw();
	for (const auto& p : players) p->draw();
	for (const auto& e : enemies) e->draw();
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