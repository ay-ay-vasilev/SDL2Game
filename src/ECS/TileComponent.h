#pragma once

#include "ECS.h"
#include <SDL.h>

class TileComponent : public Component
{
public:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, std::string path)
	{
		texture = TextureManager::LoadTexture(path);

		float scale = 6.f;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = 32;
		srcRect.h = 32;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = 32*scale;
		destRect.h = 32*scale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};