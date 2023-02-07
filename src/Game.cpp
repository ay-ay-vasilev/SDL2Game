#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Collision.h"
#include <iostream>

std::unique_ptr<Map> map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::gameEvent;

Manager manager;
auto& player(manager.addEntity());
auto& wall(manager.addEntity());

Game::Game() : window(nullptr), isRunning(false), count(0)
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

	map = std::make_unique<Map>();
	player.addComponent<TransformComponent>(2.f);
	player.addComponent<SpriteComponent>("art/player.png");
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");

	wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
	wall.addComponent<SpriteComponent>("art/dirt.png");
	wall.addComponent<ColliderComponent>("wall");
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
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	manager.refresh();
	manager.update();

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider, wall.getComponent<ColliderComponent>().collider))
	{
		player.getComponent<TransformComponent>().position = playerPos;
		std::cout << "Wall hit!\n";
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	map->DrawMap();
	manager.draw();
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
