#pragma once
#include "Particles/ParticleEmitter.h"
#include "SDL_image.h"
#include "TextureManager.h"

class ParticleManager
{
public:
	ParticleManager();
	virtual ~ParticleManager();

	void loadParticleEmitterType(const std::string& particleName);
	std::shared_ptr<ParticleEmitter> loadParticleEmitter(const std::string& particleName);

	std::shared_ptr<ParticleEmitter> addParticleEmitter(const std::string& particleName);
	void removeParticleEmitter(const std::shared_ptr<ParticleEmitter>& particleEmitter);

	void init();
	void update();
	void draw();

private:
	std::unordered_map<std::string, std::shared_ptr<ParticleEmitter>> particleEmitterTypes;
	std::vector<std::shared_ptr<ParticleEmitter>> particleEmitters;
};