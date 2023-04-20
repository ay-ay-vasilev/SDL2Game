#pragma once
#include "TextureManager.h"
#include "Sprite.h"

#include <SDL.h>

class SpriteOutline
{
public:
	SpriteOutline() {}
	~SpriteOutline()
	{
		SDL_DestroyTexture(texture);
	}

	void setTexture(SDL_Texture* texture) { this->texture = texture; }
	void draw(SDL_Rect srcRect, SDL_Rect destRect, float scale, SDL_RendererFlip spriteFlip)
	{
		SDL_Rect tempDestRect = destRect;
		tempDestRect.x = destRect.x + static_cast<int>(scale);
		TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
		tempDestRect = destRect;
		tempDestRect.x = destRect.x - static_cast<int>(scale);
		TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
		tempDestRect = destRect;
		tempDestRect.y = destRect.y + static_cast<int>(scale);
		TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
		tempDestRect = destRect;
		tempDestRect.y = destRect.y - static_cast<int>(scale);
		TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
	}
private:

	SDL_Texture* texture{nullptr};

};