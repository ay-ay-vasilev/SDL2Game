#include "Game.h"

#include "TextureManager.h"
#include "GameObject.h"
#include <iostream>

std::unique_ptr<GameObject> player;
std::unique_ptr<GameObject> enemy;

Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), count(0)
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
    
    player = std::make_unique<GameObject>("art/player.png", renderer, 0, 0);
    enemy = std::make_unique<GameObject>("art/enemy.png", renderer, 50, 50);
}

void Game::handleEvents()
{
	SDL_Event newEvent;
	SDL_PollEvent(&newEvent);
	switch (newEvent.type)
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
    player->Update();
    enemy->Update();
	std::cout << count++ << "\n";
}

void Game::render()
{
	SDL_RenderClear(renderer);
    player->Render();
    enemy->Render();
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
