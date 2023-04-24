#pragma once
#include "AssetManager.h"

#include <SDL.h>

class Sprite
{
public:
	Sprite(SDL_Surface* surface, double z) : surface(surface), z(z) {}

	Sprite(const std::string_view& surfaceId, int z) : z(z)
	{
		surface = Game::assets->getSurface(surfaceId);
	}

	const double getZ() const { return z; }
	SDL_Surface* getSurface() const { return surface; }

private:
	const double z;
	SDL_Surface* surface;
};