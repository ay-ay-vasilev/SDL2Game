#include "ParticleManager.h"

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