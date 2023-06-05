#pragma once
#include "Game.h"

#include <optional>

class Sprite;
class TextureManager
{
public:
	static SDL_Texture* loadTexture(const std::string& fileName);
	static SDL_Surface* loadSurface(const std::string& fileName);

	static void applyColorMapping(SDL_Surface* surface, const std::vector<std::pair<SDL_Color, SDL_Color>>& colorMapping);

	static SDL_Texture* getTextureFromSurface(SDL_Surface* surface, const std::optional<SDL_Color>& textureColor = std::nullopt);
	static SDL_Texture* getCombinedTexture(const std::vector<std::shared_ptr<Sprite>>& sortedSprites, const std::optional<SDL_Color>& textureColor = std::nullopt);
	static void draw(SDL_Texture* texture, const SDL_Rect source, SDL_Rect destination, const double angle = 0.f, const SDL_RendererFlip flip = SDL_FLIP_NONE, const bool moveWithCamera = true);
	static void draw(SDL_Texture* texture, const SDL_Rect* source, SDL_Rect* destination, const double angle = 0.f, const SDL_RendererFlip flip = SDL_FLIP_NONE, const bool moveWithCamera = true);

private:
	static Uint32 get_pixel(SDL_Surface* surface, int x, int y);
	static void set_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
};
