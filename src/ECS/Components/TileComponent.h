#pragma once

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

	TileComponent(const int srcX, const int srcY, const int xpos, const int ypos, const int tileSize, const std::string_view& textureId)
	{
		texture = Game::assets->getTexture(textureId);
		position = { static_cast<float>(xpos), static_cast<float>(ypos) };
		srcRect = { srcX, srcY, tileSize, tileSize };
		destRect = { xpos, ypos, tileSize * static_cast<int>(Game::manager->getScale()), static_cast<int>(tileSize * Game::manager->getScale()) };
	}

	void update() override
	{
		destRect.x = static_cast<int>(position.x - Game::camera.x);
		destRect.y = static_cast<int>(position.y - Game::camera.y);
	}

	void draw() override
	{
		TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};