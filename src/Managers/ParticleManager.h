#pragma once
#include "Particles/ParticleEmitter.h"
#include "SDL_image.h"
#include "TextureManager.h"

class ParticleManager
{
public:
	ParticleManager() {}
	virtual ~ParticleManager() {}

	std::shared_ptr<ParticleEmitter> addParticleEmitter(const std::string& particleName);
	void removeParticleEmitter(const std::shared_ptr<ParticleEmitter>& particleEmitter);

	void update();
	void draw();

private:
	std::vector<std::shared_ptr<ParticleEmitter>> particleEmitters;
};