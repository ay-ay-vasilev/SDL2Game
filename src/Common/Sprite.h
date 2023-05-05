#pragma once
#include "AssetManager.h"

#include <SDL.h>

class Sprite
{
public:
	Sprite(SDL_Surface* surface, double z) : surface(surface), z(z) {}

	Sprite(const std::string_view& surfaceId, int z, const std::optional<std::vector<std::pair<SDL_Color, SDL_Color>>>& spriteColorMapping = std::nullopt) : z(z)
	{
		if (spriteColorMapping)
			colorMapping = spriteColorMapping.value();
		surface = Game::assets->getSurface(surfaceId);
	}

	const double getZ() const { return z; }
	SDL_Surface* getSurface() const { return surface; }
	const std::vector<std::pair<SDL_Color, SDL_Color>> getColorMapping() const { return colorMapping; }

private:
	const double z;
	SDL_Surface* surface;
	std::vector<std::pair<SDL_Color, SDL_Color>> colorMapping;
};