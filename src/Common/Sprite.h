#pragma once
#include "AssetManager.h"

#include <SDL.h>

class Sprite
{
public:
	Sprite(SDL_Surface* surface, double z) : surface(surface), z(z) {}

	Sprite(const std::string_view& surfaceId, int z, const std::optional<std::string>& color = std::nullopt) : z(z)
	{
		const auto stringId = std::string(surfaceId);

		if (color) surface = Game::assetManager->getSurface(stringId, color.value());
		else surface = Game::assetManager->getSurface(stringId);
	}

	const double getZ() const { return z; }
	SDL_Surface* getSurface() const { return surface; }

private:
	const double z;
	SDL_Surface* surface;
};