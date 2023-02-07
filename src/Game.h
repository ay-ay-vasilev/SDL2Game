#pragma once
#ifdef _WIN32
#include "SDL.h"
#include "SDL_render.h"
#include "SDL_image.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2_image/SDL_image.h>
#endif
#include <memory>
#include <string>
#include <functional>

class Game {

public:
	Game();
	~Game();

	void init(std::string title, int x, int y, int width, int height, bool fullScreen);
	
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running();

	static SDL_Renderer* renderer;

private:
	bool isRunning;
	SDL_Window* window;

	int count;
};
