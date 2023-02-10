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
#include <vector>

class ColliderComponent;

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

	static void AddTile(int srcX, int srcY, int xpos, int ypos);
	static SDL_Renderer* renderer;
	static SDL_Event gameEvent;
	static std::vector<ColliderComponent*> colliders;

private:
	bool isRunning;
	SDL_Window* window;

	int count;
};
