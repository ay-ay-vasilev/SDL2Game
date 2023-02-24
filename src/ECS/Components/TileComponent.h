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

	TileComponent(const int srcX, const int srcY, const int xpos, const int ypos, const int tileSize, const int tileScale, const std::string_view textureId)
	{
		texture = Game::assets->GetTexture(textureId);
		position = { static_cast<float>(xpos), static_cast<float>(ypos) };
		srcRect = { srcX, srcY, tileSize, tileSize };
		destRect = { xpos, ypos, tileSize * tileScale, tileSize * tileScale };
	}

	void update() override
	{
		destRect.x = static_cast<int>(position.x - Game::camera.x);
		destRect.y = static_cast<int>(position.y - Game::camera.y);
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};