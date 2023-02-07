#pragma once

#include "Components.h"
#include "TextureManager.h"
#include <SDL.h>

class SpriteComponent : public Component
{
public:
	SpriteComponent() = default;
	SpriteComponent(std::string path)
	{
		setTexture(path);
	}

	void setTexture(std::string path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = 32;
		srcRect.h = 32;

		destRect.w = 64;
		destRect.h = 64;
	}

	void update() override
	{
		destRect.x = (int)transform->position.x;
		destRect.y = (int)transform->position.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect);
	}

private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
};