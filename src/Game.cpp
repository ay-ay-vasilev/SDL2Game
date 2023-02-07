#include "Game.h"
#include <iostream>

SDL_Texture* playerTex;
SDL_Rect srcR, destR;

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

		window = std::unique_ptr<SDL_Window, SDLDeleter>(
			SDL_CreateWindow(title.data(), x, y, width, height, flags)
		);

		if (window)
		{
			std::cout << "Window created!\n";
		}

		renderer = std::unique_ptr<SDL_Renderer, SDLDeleter>(
			SDL_CreateRenderer(window.get(), -1, 0)
		);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
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

	SDL_Surface* tmpSurface = IMG_Load("../art/player.png");
	playerTex = SDL_CreateTextureFromSurface(renderer.get(), tmpSurface);
	SDL_FreeSurface(tmpSurface);
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
	destR.h = 54;
	destR.w = 30;
	destR.x = count;

	std::cout << count++ << "\n";
}

void Game::render()
{
	SDL_RenderClear(renderer.get());
	SDL_RenderCopy(renderer.get(), playerTex, NULL, &destR);
	SDL_RenderPresent(renderer.get());
}

void Game::clean()
{
	SDL_DestroyWindow(window.get());
	SDL_DestroyRenderer(renderer.get());
	SDL_Quit();
	std::cout << "Game cleaned!\n";
}

bool Game::running()
{
	return isRunning;
}
