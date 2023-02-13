#pragma once

#include "ECS.h"
#include <SDL.h>
#include "Vector2D.h"
#include "Game.h"
#include "TextureManager.h"

class TileComponent : public Component
{
public:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tileSize, int tileScale, std::string textureId)
	{
		texture = Game::assets->GetTexture(textureId);

		position.x = xpos;
		position.y = ypos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = tileSize;
		srcRect.h = tileSize;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = tileSize * tileScale;
		destRect.h = tileSize * tileScale;
	}

	void update() override
	{
		destRect.x = position.x - Game::camera.x;
		destRect.y = position.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};