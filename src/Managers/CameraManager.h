#pragma once
#include "SDL.h"

#include <vector>
#include <memory>

namespace ecs
{
	class Manager;
	class Entity;
}

class CameraManager
{
public:
	explicit CameraManager(std::shared_ptr<ecs::Manager> manager);
	~CameraManager();

	// Disallow copy
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;

	// Disallow move
	CameraManager(CameraManager&&) = delete;
	CameraManager& operator=(CameraManager&&) = delete;

	void init();
	void update();
	const SDL_Rect getCameraPosition() inline const { return camera; }
	const bool isInView(const SDL_Rect& destination) const;

private:
	SDL_Rect camera;
	SDL_Rect cameraBounds;

	std::shared_ptr<ecs::Manager> manager;
	std::vector<ecs::Entity*> cameraEntities;
};