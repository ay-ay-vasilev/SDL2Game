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
	}

	SDL_Texture* texture;
	int frameWidth;
	int frameHeight;
	double z;

	SDL_Rect srcRect, destRect;
};