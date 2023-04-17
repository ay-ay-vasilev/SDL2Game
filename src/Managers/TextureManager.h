#pragma once
#include "Game.h"

class TextureManager
{
public:
	static SDL_Texture* loadTexture(const std::string& fileName);
	static SDL_Surface* loadSurface(const std::string& fileName);
	static SDL_Texture* getTextureFromSurface(SDL_Surface* surface);
	static void draw(SDL_Texture* texture, const SDL_Rect source, const SDL_Rect destination, const SDL_RendererFlip flip);
};
