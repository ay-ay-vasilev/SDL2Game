#pragma once
#include "Game.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const std::string_view& fileName);
	static void Draw(SDL_Texture* tex, const SDL_Rect src, const SDL_Rect dest, const SDL_RendererFlip flip);
};
