#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include <SDL.h>

class TileComponent : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	SDL_Rect tileRect;
	int tileID;
	std::string path;

	TileComponent() = default;
	TileComponent(int x, int y, int w, int h, int id) : tileRect(SDL_Rect(x, y, w, h)), tileID(id)
	{
		switch (tileID)
		{
		case 0:
			path = "art/dirt.png";
			break;
		case 1:
			path = "art/grass.png";
			break;
		case 2:
			path = "art/water.png";
			break;
		default:
			break;
		}
	}

	void init() override
	{
		entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 1.f);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();
	}
};