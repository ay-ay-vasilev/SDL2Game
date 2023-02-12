#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Collision.h"
#include <iostream>

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::gameEvent;

SDL_Rect Game::camera = { 0, 0, 16 * 32 * 6 - 800, 16 * 32 * 6 - 640};
Map* map;

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

	map = new Map("art/tiles_v0.png", 6, 32);

	map->LoadMap("map", 16, 16);

	player.addComponent<TransformComponent>(6.f);
	player.addComponent<SpriteComponent>("art/goblin_downscale_spritesheet.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(eGroupLabels::PLAYERS);
}

auto& tiles(manager.getGroup(Game::eGroupLabels::MAP));
auto& players(manager.getGroup(Game::eGroupLabels::PLAYERS));
auto& enemies(manager.getGroup(Game::eGroupLabels::ENEMIES));
auto& colliders(manager.getGroup(Game::eGroupLabels::COLLIDERS));

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

	for (auto& c : colliders)
	{
		SDL_Rect collider = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(collider, playerCollider))
		{
			player.getComponent<TransformComponent>().position = playerPosition;
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