#pragma once
#include <SDL.h>

class Sprite
{
public:
	Sprite(SDL_Texture* texture, int frameWidth, int frameHeight, double z) : texture(texture), frameWidth(frameWidth), frameHeight(frameHeight), z(z)
	{
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;

		destRect = SDL_Rect();
	}

	Sprite(const std::string_view& textureId, int frameWidth, int frameHeight, int z) : frameWidth(frameWidth), frameHeight(frameHeight), z(z)
	{
		texture = Game::assets->getTexture(textureId);

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;

		destRect = SDL_Rect();
	}
		
	SDL_Texture* texture;
	int frameWidth;
	int frameHeight;
	double z;

	SDL_Rect srcRect, destRect;
};