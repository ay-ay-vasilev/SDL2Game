#pragma once
#ifdef _WIN32
#else
#endif
#include "Game.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const std::string& fileName, SDL_Renderer* ren);
};