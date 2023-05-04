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

namespace ecs
{
	class Manager;
}


class AssetManager;
class KeyboardManager;
class CameraManager;
class Constants;

class Game {

public:
	Game();
	~Game();

	void init();
	
	void handleEvents();
	void update(double delta);
	void render();
	void clean();
	bool running();

	static SDL_Renderer* renderer;
	static SDL_Event gameEvent;
	static bool isRunning;

	static std::shared_ptr<Constants> constants;

	static std::unique_ptr<AssetManager> assets;
	static std::unique_ptr<KeyboardManager> keyboardManager;
	static std::unique_ptr<CameraManager> cameraManager;
	static std::shared_ptr<ecs::Manager> manager;

	enum eGroupLabels : std::size_t
	{
		MAP,
		PLAYERS,
		ACTORS,
		COLLIDERS,
		PROJECTILES
	};

private:
	SDL_Window* window;

	int count;
};
