#include "SDL.h"
#include "Game.h"
#include <memory>

std::unique_ptr<Game> game;

int main(int argc, char* argv[])
{
	game = std::make_unique<Game>();
	game->init("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	while (game->running())
	{
		game->handleEvents();
		game->update();
		game->render();
	}

	game->clean();

	return 0;
}