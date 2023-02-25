#pragma once
#include <string>
#include <SDL.h>
#include "Components.h"
#include "TextureManager.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent(const std::string_view tag) :
		tag(tag),
		collider() {};

	ColliderComponent(const std::string_view tag, const int xpos, const int ypos, const int size) :
		tag(tag),
		collider({xpos, ypos, size, size}) {};

	SDL_Rect collider;
	std::string tag;

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	TransformComponent* transform;

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		texture = TextureManager::loadTexture("assets/images/collider.png");
		srcRect = { 0, 0, collider.w, collider.h };
		destRect = { collider.x, collider.y, collider.w, collider.h };
	}

	void update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = static_cast<int>(transform->width * transform->scale);
			collider.h = static_cast<int>(transform->height * transform->scale);
		}

		destRect.x = collider.x - Game::camera.x;
		destRect.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		// todo: add debug logic ?
		//TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};