#include "ParticleManager.h"
#include "AssetManager.h"

#include "wrappedJson.h"

ParticleManager::ParticleManager() {}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::init()
{
	const auto particlesData = assets::getGeneralDataJson("particles")["particles"];
	for (const auto particleName : particlesData)
	{
		loadParticleEmitterType(particleName);
	}
}

void ParticleManager::loadParticleEmitterType(const std::string& particleName)
{
	auto particleEmitter = loadParticleEmitter(particleName);
	particleEmitterTypes[particleName] = particleEmitter;
}

std::shared_ptr<ParticleEmitter> ParticleManager::loadParticleEmitter(const std::string& particleName)
{
	auto particleEmitter = std::make_shared<ParticleEmitter>();
	nlohmann::json particleData;
	particleData = assets::getParticleDataJson(particleName);

	// texture
	if (particleData.contains("texture"))
		particleEmitter->setTexture(TextureManager::getTextureFromSurface(Game::assetManager->getSurface(particleData["texture"].get<std::string>())));

	// max particles
	particleEmitter->initWithTotalParticles(particleData["maxParticles"].get<int>());

	// duration
	if (particleData.contains("duration"))
		particleEmitter->setDuration(particleData["duration"].get<float>());

	// mode
	if (particleData.contains("mode"))
	{
		if (particleData["mode"] == "GRAVITY")
			particleEmitter->setEmitterMode(ParticleEmitter::eMode::GRAVITY);
		else if (particleData["mode"] == "RADIUS")
			particleEmitter->setEmitterMode(ParticleEmitter::eMode::RADIUS);
		else
			assert(false && "Invalid particle mode");
	}

	// Gravity Mode
	if (particleEmitter->getEmitterMode() == ParticleEmitter::eMode::GRAVITY)
	{
		if (particleData.contains("gravity"))
		{
			particleEmitter->setGravity({ particleData["gravity"]["x"].get<float>(), particleData["gravity"]["y"].get<float>() });
		}

		if (particleData.contains("radialAccel"))
		{
			particleEmitter->setRadialAccel(particleData["radialAccel"]["value"].get<float>());
			particleEmitter->setRadialAccelVar(particleData["radialAccel"]["variance"].get<float>());
		}
		if (particleData.contains("tangentialAccel"))
		{
			particleEmitter->setTangentialAccel(particleData["tangentialAccel"]["value"].get<float>());
			particleEmitter->setTangentialAccelVar(particleData["tangentialAccel"]["variance"].get<float>());
		}
		if (particleData.contains("speed"))
		{
			particleEmitter->setSpeed(particleData["speed"]["value"].get<float>());
			particleEmitter->setSpeedVar(particleData["speed"]["variance"].get<float>());
		}
	}
	// Radius Mode
	else if (particleEmitter->getEmitterMode() == ParticleEmitter::eMode::RADIUS)
	{
		if (particleData.contains("startRadius"))
		{
			particleEmitter->setStartRadius(particleData["startRadius"]["value"].get<float>());
			particleEmitter->setStartRadiusVar(particleData["startRadius"]["variance"].get<float>());
		}
		if (particleData.contains("endRadius"))
		{
			particleEmitter->setEndRadius(particleData["endRadius"]["value"].get<float>());
			particleEmitter->setEndRadiusVar(particleData["endRadius"]["variance"].get<float>());
		}
		if (particleData.contains("rotatePerSecond"))
		{
			particleEmitter->setRotatePerSecond(particleData["rotatePerSecond"]["value"].get<float>());
			particleEmitter->setRotatePerSecondVar(particleData["rotatePerSecond"]["variance"].get<float>());
		}
	}

	// angle
	if (particleData.contains("angle"))
	{
		particleEmitter->setAngle(particleData["angle"]["value"].get<float>());
		particleEmitter->setAngleVar(particleData["angle"]["variance"].get<float>());
	}

	// life of particles
	if (particleData.contains("life"))
	{
		particleEmitter->setLife(particleData["life"]["value"].get<float>());
		particleEmitter->setLifeVar(particleData["life"]["variance"].get<float>());
	}
	// emits per frame
	particleEmitter->setEmissionRate(particleEmitter->getTotalParticles() / particleEmitter->getLife());

	// color of particles
	if (particleData.contains("startColor"))
	{
		particleEmitter->setStartColor(
		{
			particleData["startColor"]["value"]["r"].get<float>(),
			particleData["startColor"]["value"]["g"].get<float>(),
			particleData["startColor"]["value"]["b"].get<float>(),
			particleData["startColor"]["value"]["a"].get<float>()
		});
		particleEmitter->setStartColorVar(
		{
			particleData["startColor"]["variance"]["r"].get<float>(),
			particleData["startColor"]["variance"]["g"].get<float>(),
			particleData["startColor"]["variance"]["b"].get<float>(),
			particleData["startColor"]["variance"]["a"].get<float>()
		});
	}
	if (particleData.contains("endColor"))
	{
		particleEmitter->setEndColor(
		{
			particleData["endColor"]["value"]["r"].get<float>(),
			particleData["endColor"]["value"]["g"].get<float>(),
			particleData["endColor"]["value"]["b"].get<float>(),
			particleData["endColor"]["value"]["a"].get<float>()
		});
		particleEmitter->setEndColorVar(
		{
			particleData["endColor"]["variance"]["r"].get<float>(),
			particleData["endColor"]["variance"]["g"].get<float>(),
			particleData["endColor"]["variance"]["b"].get<float>(),
			particleData["endColor"]["variance"]["a"].get<float>()
		});
	}
	// size, in pixels
	if (particleData.contains("startSize"))
	{
		particleEmitter->setStartSize(particleData["startSize"]["value"].get<float>());
		particleEmitter->setStartSizeVar(particleData["startSize"]["variance"].get<float>());
	}
	if (particleData.contains("endSize"))
	{
		particleEmitter->setEndSize(particleData["endSize"]["value"].get<float>());
		particleEmitter->setEndSizeVar(particleData["endSize"]["variance"].get<float>());
	}
	// position
	if (particleData.contains("pos"))
	{
		particleEmitter->setPosVar({ particleData["pos"]["x"].get<float>(), particleData["pos"]["y"].get<float>() });
	}
	// spinning
	if (particleData.contains("startSpin"))
	{
		particleEmitter->setStartSpin(particleData["startSpin"]["value"].get<float>());
		particleEmitter->setStartSpinVar(particleData["startSpin"]["variance"].get<float>());
	}
	if (particleData.contains("endSpin"))
	{
		particleEmitter->setEndSpin(particleData["endSpin"]["value"].get<float>());
		particleEmitter->setEndSpinVar(particleData["endSpin"]["variance"].get<float>());
	}
	return particleEmitter;
}

std::shared_ptr<ParticleEmitter> ParticleManager::addParticleEmitter(const std::string& particleName)
{
	const auto particleEmitter = particleEmitterTypes[particleName];
	const auto newEmitter = std::make_shared<ParticleEmitter>(*particleEmitter);
	newEmitter->setTexture(TextureManager::getTextureFromSurface(Game::assetManager->getSurface("pixel")));
	particleEmitters.push_back(newEmitter);
	return newEmitter;
}

void ParticleManager::removeParticleEmitter(const std::shared_ptr<ParticleEmitter>& particleEmitter)
{
	particleEmitter->stopSystem();
	particleEmitters.erase(std::remove(particleEmitters.begin(), particleEmitters.end(), particleEmitter), particleEmitters.end());
}

void ParticleManager::update()
{
	particleEmitters.erase(
		std::remove_if(particleEmitters.begin(), particleEmitters.end(),
			[](const std::shared_ptr<ParticleEmitter>& emitter) {
				return !emitter->isActive() && emitter->getParticleCount() == 0;
			}),
		particleEmitters.end()
	);

	const int numEmitters = particleEmitters.size();
	const int numThreads = SDL_GetCPUCount();

	if (numEmitters < numThreads)
	{
		for (auto& particleEmitter : particleEmitters)
		{
			particleEmitter->update();
		}
	}
	else
	{
		const int emittersPerThread = numEmitters / numThreads;
		const int remainingEmitters = numEmitters % numThreads;

		std::vector<SDL_Thread*> threads(numThreads);

		int emitterIndex = 0;

		for (int i = 0; i < numThreads; ++i)
		{
			int numEmittersInBatch = emittersPerThread;
			if (i < remainingEmitters)
			{
				numEmittersInBatch++;
			}

			auto emittersBatch = new std::vector<std::shared_ptr<ParticleEmitter>>(
				particleEmitters.begin() + emitterIndex,
				particleEmitters.begin() + emitterIndex + numEmittersInBatch
			);

			threads[i] = SDL_CreateThread([](void* data) {
				auto emittersBatch = static_cast<std::vector<std::shared_ptr<ParticleEmitter>>*>(data);

				for (auto& particleEmitter : *emittersBatch)
				{
					particleEmitter->update();
				}

				delete emittersBatch;

				return 0;
				}, "ParticleThread", emittersBatch);

			emitterIndex += numEmittersInBatch;
		}

		for (int i = 0; i < numThreads; ++i)
		{
			int threadReturnValue;
			SDL_WaitThread(threads[i], &threadReturnValue);
		}
	}
}