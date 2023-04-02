#pragma once
#include <SDL.h>

class Sprite
{
public:
	Sprite(SDL_Texture* texture, int frameWidth, int frameHeight, double z) : texture(texture), frameWidth(frameWidth), frameHeight(frameHeight), z(z)
	{
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;

		destRect = SDL_Rect();
	}

	Sprite(const std::string_view& textureId, int frameWidth, int frameHeight, int z) : frameWidth(frameWidth), frameHeight(frameHeight), z(z)
	{
		texture = Game::assets->getTexture(textureId);

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;

		destRect = SDL_Rect();
	}
	
	void setAnimNameIndexes(const nlohmann::json& animData)
	{
		for (auto& [name, animData] : animData.items())
			animNameIndex[name] = animData.value("anim_index", -1);
	}

	int getIndexByAnimName(const std::string& animName)
	{
		if (animNameIndex.count(animName) != 0)
			return animNameIndex[animName];
		return -1;
	}

	const double getZ() const { return z; }
	const SDL_Rect getSrcRect() const { return srcRect; }
	const SDL_Rect getDestRect() const { return destRect; }
	SDL_Texture* getTexture() const { return texture; }

	void setSrcRectXForFrame(const int frameNum)
	{
		srcRect.x = srcRect.w * frameNum;
	}
	
	void setSrcRectYForAnim(const std::string& animName, const int animIndex)
	{
		const auto index = getIndexByAnimName(animName); // in case a Sprite has a different mapping of animIndex-animName than the general SpriteComponent mapping
		srcRect.y = (index != -1 ? index : animIndex) * frameHeight;
	}

	void setDestRect(const Vector2D& transformPosition, const Vector2D& cameraPosition, const int transformScale)
	{
		destRect.x = static_cast<int>(transformPosition.x - static_cast<float>(frameWidth) * transformScale / 2) - cameraPosition.x;
		destRect.y = static_cast<int>(transformPosition.y - static_cast<float>(frameHeight) * transformScale / 2) - cameraPosition.y;
		destRect.w = static_cast<int>(frameWidth * transformScale);
		destRect.h = static_cast<int>(frameHeight * transformScale);
	}

private:
	const int frameWidth;
	const int frameHeight;
	const double z;

	std::unordered_map<std::string, int> animNameIndex;

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
};