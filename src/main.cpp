#include <SDL2/SDL.h>
#include "Game.h"
#include <memory>
#include <chrono>
#include <thread>

std::unique_ptr<Game> game;

int main(int argc, char* argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	game = std::make_unique<Game>();
	game->init("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	auto nextFrame = std::chrono::steady_clock::now();
	while (game->running())
	{
		game->handleEvents();
		game->update();
		game->render();

		nextFrame += std::chrono::milliseconds(frameDelay);
		std::this_thread::sleep_until(nextFrame);
	}
	game->clean();

	return 0;
}
