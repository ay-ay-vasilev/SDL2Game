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
class EnemyManager;
class Manager;
class Constants;

class Game {

public:
	Game();
	~Game();

	void init();
	
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running();

	static SDL_Renderer* renderer;
	static SDL_Event gameEvent;
	static bool isRunning;
	static SDL_Rect camera;
	static int cameraMinX;
	static int cameraMaxX;
	static int cameraMinY;
	static int cameraMaxY;

	static std::shared_ptr<Constants> constants;

	static std::unique_ptr<AssetManager> assets;
	static std::unique_ptr<EnemyManager> enemyManager;
	static std::shared_ptr<Manager> manager;

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
