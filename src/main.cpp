#ifdef _WIN32
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif
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
	game->init();

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
