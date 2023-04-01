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
			animNameIndex[name] = animData.value("id", -1);
	}

	int getIndexByAnimName(const std::string& animName)
	{
		if (animNameIndex.count(animName) != 0)
			return animNameIndex[animName];
		return -1;
	}

	SDL_Texture* texture;
	const int frameWidth;
	const int frameHeight;
	const double z;

	std::unordered_map<std::string, int> animNameIndex;

	SDL_Rect srcRect, destRect;
};