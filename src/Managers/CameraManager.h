#pragma once
#include "SDL.h"

#include <vector>
#include <memory>
#include "Vector2D.h"

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
	const SDL_Rect getCameraBounds() inline const { return cameraBounds; }
	const bool isInView(const SDL_Rect& destination) const;

private:
	const Vector2D getSumCameraPosition() const;
	void updatePositionsOnScreen() const;

	SDL_Rect camera;
	SDL_Rect cameraBounds;

	int screenWidth, screenHeight;

	std::shared_ptr<ecs::Manager> manager;
	std::vector<ecs::Entity*> cameraEntities;
};