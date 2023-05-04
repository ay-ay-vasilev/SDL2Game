#pragma once
#include "Game.h"

#include <optional>

class Sprite;
class TextureManager
{
public:
	static SDL_Texture* loadTexture(const std::string& fileName);
	static SDL_Surface* loadSurface(const std::string& fileName);
	static SDL_Texture* getTextureFromSurface(SDL_Surface* surface);
	static SDL_Texture* getCombinedTexture(const std::vector<std::shared_ptr<Sprite>>& sortedSprites, const std::optional<SDL_Color>& textureColor = std::nullopt);
	static void draw(SDL_Texture* texture, const SDL_Rect source, SDL_Rect destination, const SDL_RendererFlip flip);
};
