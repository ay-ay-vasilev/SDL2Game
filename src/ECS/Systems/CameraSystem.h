#pragma once
#include "ECS.h"

#include "SDL.h"

class CameraSystem : public System
{
public:
	CameraSystem(Manager& manager);

	void init() override;
	void update() override;
	void draw() override;

private:
	SDL_Rect camera;
	SDL_Rect cameraBounds;

	std::vector<Entity*> players;
	Vector2D lastPlayerPosition;
};