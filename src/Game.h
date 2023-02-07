#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <memory>
#include <string>
#include <functional>

class SDLDeleter {
public:
	void operator()(SDL_Window* window) {
		SDL_DestroyWindow(window);
	}
	void operator()(SDL_Renderer* renderer) {
		SDL_DestroyRenderer(renderer);
	}
};

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
	std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> window;
	std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>> renderer;

	int count;
};
