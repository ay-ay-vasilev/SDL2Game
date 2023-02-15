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
class AssetManager;

class Game {

public:
	Game();
	~Game();

	void init(const std::string_view title, const int x, const int y, const int width, const int height, const bool fullScreen);
	
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running();

	static SDL_Renderer* renderer;
	static SDL_Event gameEvent;
	static bool isRunning;
	static SDL_Rect camera;
	static AssetManager* assets;

	enum eGroupLabels : std::size_t
	{
		MAP,
		PLAYERS,
		ENEMIES,
		COLLIDERS,
		PROJECTILES
	};

private:
	SDL_Window* window;

	int count;
};
