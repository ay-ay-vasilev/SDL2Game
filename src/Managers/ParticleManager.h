#pragma once
#include "Particles/ParticleSystem.h"
#include "SDL_image.h"
#include "TextureManager.h"

class ParticleManager : public ParticleSystem
{
public:
	ParticleManager() {}
	virtual ~ParticleManager() {}

	enum ParticleStyle
	{
		BLOOD
	};

	ParticleStyle style_ = BLOOD;
	void setStyle(ParticleStyle style);

	void loadParticleData(const std::string& particleName);

	SDL_Texture* getDefaultTexture()
	{
		std::string texturePath = "assets/images/particles/pixel.png";
		static SDL_Texture* texture = TextureManager::loadTexture(texturePath);
		return texture;
	}
};