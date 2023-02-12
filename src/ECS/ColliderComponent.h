#pragma once
#include <string>
#include <SDL.h>
#include "Components.h"
#include "TextureManager.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent(std::string tag) :
		tag(tag),
		collider() {};

	ColliderComponent(std::string tag, int xpos, int ypos, int size) :
		tag(tag),
		collider({xpos, ypos, size, size}) {};

	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcRect, destRect;

	TransformComponent* transform;

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::LoadTexture("art/collider.png");
		srcRect = { 0, 0, 32, 32 };
		destRect = { collider.x, collider.y, collider.w, collider.h };
	}

	void update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}

		destRect.x = collider.x - Game::camera.x;
		destRect.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(tex, srcRect, destRect, SDL_FLIP_NONE);
	}
};