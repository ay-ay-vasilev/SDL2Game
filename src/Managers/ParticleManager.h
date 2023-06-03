#pragma once
#include "Particles/ParticleSystem.h"
#include "SDL_image.h"

#include <filesystem>

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
	SDL_Texture* getDefaultTexture()
	{
		std::filesystem::path p = std::filesystem::current_path() / ".." / "assets/images/misc/particle_tutor/particle.png";
		static SDL_Texture* t = IMG_LoadTexture(_renderer, (p.string()).c_str());
		return t;
	}
};