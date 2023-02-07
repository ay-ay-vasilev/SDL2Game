#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Collision.h"
#include <iostream>

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::gameEvent;

std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());

enum eGroupLabels : std::size_t
{
	MAP,
	PLAYERS,
	ENEMIES,
	COLLIDERS
};

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

	Map::LoadMap("../art/map.txt", 16, 16);

	player.addComponent<TransformComponent>(2.f);
	player.addComponent<SpriteComponent>("art/player_idle.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(eGroupLabels::PLAYERS);

	wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
	wall.addComponent<SpriteComponent>("art/dirt.png");
	wall.addComponent<ColliderComponent>("wall");
	wall.addGroup(eGroupLabels::MAP);
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

	for (const auto collider : colliders)
	{
		Collision::AABB(player.getComponent<ColliderComponent>(), *collider);
	}
}

auto& tiles(manager.getGroup(eGroupLabels::MAP));
auto& players(manager.getGroup(eGroupLabels::PLAYERS));
auto& enemies(manager.getGroup(eGroupLabels::ENEMIES));

void Game::render()
{
	SDL_RenderClear(renderer);
	for (const auto& t : tiles) t->draw();
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

void Game::AddTile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);
	tile.addGroup(eGroupLabels::MAP);
}