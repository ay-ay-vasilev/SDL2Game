#pragma once
#include <SDL.h>

class Sprite
{
public:
	Sprite(SDL_Surface* surface, int frameWidth, int frameHeight, double z) : surface(surface), frameWidth(frameWidth), frameHeight(frameHeight), z(z)
	{
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;

		destRect = SDL_Rect();
	}

	Sprite(const std::string_view& textureId, int frameWidth, int frameHeight, int z) : frameWidth(frameWidth), frameHeight(frameHeight), z(z)
	{
		surface = Game::assets->getSurface(textureId);

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;

		destRect = SDL_Rect();
	}

	const double getZ() const { return z; }
	const SDL_Rect getSrcRect() const { return srcRect; }
	const SDL_Rect getDestRect() const { return destRect; }
	SDL_Surface* getSurface() const { return surface; }

	void setSrcRectXForFrame(const int frameNum)
	{
		srcRect.x = srcRect.w * frameNum;
	}
	
	void setSrcRectYForAnim(const std::string& animName, const int animIndex)
	{
		srcRect.y = (animIndex) * frameHeight;
	}

	void setDestRect(const Vector2D& transformPosition, const Vector2D& cameraPosition, const float transformScale)
	{
		destRect.x = static_cast<int>(transformPosition.x - static_cast<float>(frameWidth) * transformScale / 2.f - cameraPosition.x);
		destRect.y = static_cast<int>(transformPosition.y - static_cast<float>(frameHeight) * transformScale / 2.f - cameraPosition.y);
		destRect.w = static_cast<int>(frameWidth * transformScale);
		destRect.h = static_cast<int>(frameHeight * transformScale);
	}

private:
	const int frameWidth;
	const int frameHeight;
	const double z;

	SDL_Surface* surface;
	SDL_Rect srcRect, destRect;
};