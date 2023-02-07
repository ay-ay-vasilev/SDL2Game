#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2_image/SDL_image.h>
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

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

	int count;
};
