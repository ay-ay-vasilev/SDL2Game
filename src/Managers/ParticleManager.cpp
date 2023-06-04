#include "ParticleManager.h"
#include "AssetManager.h"

#include "wrappedJson.h"

void ParticleManager::setStyle(ParticleStyle style)
{
	style_ = style;
	if (_texture == nullptr)
	{
		setTexture(getDefaultTexture());
	}
	switch (style)
	{
	case ParticleManager::BLOOD:
	{
		initWithTotalParticles(500);

		// duration
		_duration = 0.3f;

		// Gravity Mode
		this->_emitterMode = eMode::GRAVITY;

		// Gravity Mode: gravity
		this->modeA.gravity = { 0.0f, 90.0f };

		// Gravity Mode:  radial
		this->modeA.radialAccel = 0.0f;
		this->modeA.radialAccelVar = 0.0f;

		//  Gravity Mode: speed of particles
		this->modeA.speed = -90.0f;
		this->modeA.speedVar = 25.0f;

		// angle
		//this->_angle = 45.0f;
		this->_angleVar = 20.0f;

		// life of particles
		this->_life = 1.5f;
		this->_lifeVar = 1.0f;

		// emits per frame
		this->_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 0.8f;
		_startColor.g = 0.0f;
		_startColor.b = 0.0f;
		_startColor.a = 0.0f;
		_startColorVar.r = 0.1f;
		_startColorVar.g = 0.1f;
		_startColorVar.b = 0.1f;
		_startColorVar.a = 0.0f;

		_endColor.r = 0.2f;
		_endColor.g = 0.0f;
		_endColor.b = 0.0f;
		_endColor.a = 1.0f;
		_endColorVar.r = 0.1f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;

		// size, in pixels
		_startSize = 1.0f;
		_startSizeVar = 1.0f;
		_endSize = 1.0f;
		_endSizeVar = 0.0f;

		_posVar = { 0, 0 };
		break;
	}
	default:
		break;
	}
}

void ParticleManager::loadParticleData(const std::string& particleName)
{
	nlohmann::json particleData;
	particleData = assets::getParticleDataJson(particleName);

	initWithTotalParticles(particleData["maxParticles"].get<int>());

	// duration
	if (particleData.contains("duration"))
		_duration = particleData["duration"].get<float>();

	// Gravity Mode
	if (particleData.contains("mode"))
	{
		if (particleData["mode"] == "GRAVITY")
			this->_emitterMode = eMode::GRAVITY;
		else if (particleData["mode"] == "RADIUS")
			this->_emitterMode = eMode::RADIUS;
		else
			assert(false && "Invalid particle mode");
	}

	if (this->_emitterMode == eMode::GRAVITY)
	{
		// Gravity Mode: gravity
		if (particleData.contains("gravity"))
		{
			this->modeA.gravity = { particleData["gravity"]["x"].get<float>(), particleData["gravity"]["y"].get<float>() };
		}
		// Gravity Mode:  radial

		if (particleData.contains("radialAccel"))
		{
			this->modeA.radialAccel = particleData["radialAccel"]["value"].get<float>();
			this->modeA.radialAccelVar = particleData["radialAccel"]["variance"].get<float>();
		}
		//  Gravity Mode: speed of particles
		if (particleData.contains("speed"))
		{
			this->modeA.speed = particleData["speed"]["value"].get<float>();
			this->modeA.speedVar = particleData["speed"]["variance"].get<float>();
		}
	}
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

	if (particleData.contains("angle"))
	{
		// angle
		this->_angle = particleData["angle"]["value"].get<float>();
		this->_angleVar = particleData["angle"]["variance"].get<float>();
	}

	if (particleData.contains("life"))
	{
		// life of particles
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
	if (particleData.contains("pos"))
	{
		this->_posVar = { particleData["pos"]["x"].get<float>(), particleData["pos"]["y"].get<float>() };
	}
}