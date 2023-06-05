#include "ParticleManager.h"
#include "AssetManager.h"

#include "wrappedJson.h"

void ParticleManager::loadParticleData(const std::string& particleName)
{
	nlohmann::json particleData;
	particleData = assets::getParticleDataJson(particleName);

	// texture
	if (particleData.contains("texture")) setTexture(TextureManager::loadTexture(particleData["texture"].get<std::string>()));

	if (_texture == nullptr)
	{
		setTexture(getDefaultTexture());
	}

	// max particles
	initWithTotalParticles(particleData["maxParticles"].get<int>());

	// duration
	if (particleData.contains("duration"))
		setDuration(particleData["duration"].get<float>());

	// mode
	if (particleData.contains("mode"))
	{
		if (particleData["mode"] == "GRAVITY")
			this->_emitterMode = eMode::GRAVITY;
		else if (particleData["mode"] == "RADIUS")
			this->_emitterMode = eMode::RADIUS;
		else
			assert(false && "Invalid particle mode");
	}

	// Gravity Mode
	if (this->_emitterMode == eMode::GRAVITY)
	{
		if (particleData.contains("gravity"))
		{
			this->modeA.gravity = { particleData["gravity"]["x"].get<float>(), particleData["gravity"]["y"].get<float>() };
		}

		if (particleData.contains("radialAccel"))
		{
			this->modeA.radialAccel = particleData["radialAccel"]["value"].get<float>();
			this->modeA.radialAccelVar = particleData["radialAccel"]["variance"].get<float>();
		}
		if (particleData.contains("tangentialAccel"))
		{
			this->modeA.tangentialAccel = particleData["tangentialAccel"]["value"].get<float>();
			this->modeA.tangentialAccelVar = particleData["tangentialAccel"]["variance"].get<float>();
		}
		if (particleData.contains("speed"))
		{
			this->modeA.speed = particleData["speed"]["value"].get<float>();
			this->modeA.speedVar = particleData["speed"]["variance"].get<float>();
		}
	}
	// Radius Mode
	else if (this->_emitterMode == eMode::RADIUS)
	{
		if (particleData.contains("startRadius"))
		{
			this->modeB.startRadius = particleData["startRadius"]["value"].get<float>();
			this->modeB.startRadiusVar = particleData["startRadius"]["variance"].get<float>();
		}

		if (particleData.contains("endRadius"))
		{
			this->modeB.endRadius = particleData["endRadius"]["value"].get<float>();
			this->modeB.endRadiusVar = particleData["endRadius"]["variance"].get<float>();
		}

		if (particleData.contains("rotatePerSecond"))
		{
			this->modeB.rotatePerSecond = particleData["rotatePerSecond"]["value"].get<float>();
			this->modeB.rotatePerSecondVar = particleData["rotatePerSecond"]["variance"].get<float>();
		}
	}

	// angle
	if (particleData.contains("angle"))
	{
		this->_angle = particleData["angle"]["value"].get<float>();
		this->_angleVar = particleData["angle"]["variance"].get<float>();
	}

	// life of particles
	if (particleData.contains("life"))
	{
		this->_life = particleData["life"]["value"].get<float>();
		this->_lifeVar = particleData["life"]["variance"].get<float>();
	}
	// emits per frame
	this->_emissionRate = _totalParticles / _life;

	// color of particles
	if (particleData.contains("startColor"))
	{
		this->_startColor.r = particleData["startColor"]["value"]["r"].get<float>();
		this->_startColor.g = particleData["startColor"]["value"]["g"].get<float>();
		this->_startColor.b = particleData["startColor"]["value"]["b"].get<float>();
		this->_startColor.a = particleData["startColor"]["value"]["a"].get<float>();
		this->_startColorVar.r = particleData["startColor"]["variance"]["r"].get<float>();
		this->_startColorVar.g = particleData["startColor"]["variance"]["g"].get<float>();
		this->_startColorVar.b = particleData["startColor"]["variance"]["b"].get<float>();
		this->_startColorVar.a = particleData["startColor"]["variance"]["a"].get<float>();
	}
	if (particleData.contains("endColor"))
	{
		this->_endColor.r = particleData["endColor"]["value"]["r"].get<float>();
		this->_endColor.g = particleData["endColor"]["value"]["g"].get<float>();
		this->_endColor.b = particleData["endColor"]["value"]["b"].get<float>();
		this->_endColor.a = particleData["endColor"]["value"]["a"].get<float>();
		this->_endColorVar.r = particleData["endColor"]["variance"]["r"].get<float>();
		this->_endColorVar.g = particleData["endColor"]["variance"]["g"].get<float>();
		this->_endColorVar.b = particleData["endColor"]["variance"]["b"].get<float>();
		this->_endColorVar.a = particleData["endColor"]["variance"]["a"].get<float>();
	}
	// size, in pixels
	if (particleData.contains("startSize"))
	{
		this->_startSize = particleData["startSize"]["value"].get<float>();
		this->_startSizeVar = particleData["startSize"]["variance"].get<float>();
	}
	if (particleData.contains("endSize"))
	{
		this->_endSize = particleData["endSize"]["value"].get<float>();
		this->_endSizeVar = particleData["endSize"]["variance"].get<float>();
	}
	// position
	if (particleData.contains("pos"))
	{
		this->_posVar = { particleData["pos"]["x"].get<float>(), particleData["pos"]["y"].get<float>() };
	}
	// spinning
	if (particleData.contains("startSpin"))
	{
		this->_startSpin = particleData["startSpin"]["value"].get<float>();
		this->_startSpinVar = particleData["startSpin"]["variance"].get<float>();
	}
	if (particleData.contains("endSpin"))
	{
		this->_endSpin = particleData["endSpin"]["value"].get<float>();
		this->_endSpinVar = particleData["endSpin"]["variance"].get<float>();
	}
}