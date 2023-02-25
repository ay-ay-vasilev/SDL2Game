#pragma once
#include "Game.h"

class TextureManager
{
public:
	static SDL_Texture* loadTexture(const std::string_view& fileName);
	static void draw(SDL_Texture* texture, const SDL_Rect source, const SDL_Rect destination, const SDL_RendererFlip flip);
};
