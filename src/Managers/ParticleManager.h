#pragma once
#include "Particles/ParticleEmitter.h"
#include "SDL_image.h"
#include "TextureManager.h"

class ParticleManager
{
public:
	void loadParticleEmitterType(const std::string& particleName);
	std::shared_ptr<particles::ParticleEmitter> loadParticleEmitter(const std::string& particleName);

	std::shared_ptr<particles::ParticleEmitter> addParticleEmitter(const std::string& particleName);
	void removeParticleEmitter(const std::shared_ptr<particles::ParticleEmitter>& particleEmitter);

	void init();
	void update();

private:
	std::unordered_map<std::string, std::shared_ptr<particles::ParticleEmitter>> particleEmitterTypes;
	std::vector<std::shared_ptr<particles::ParticleEmitter>> particleEmitters;
};