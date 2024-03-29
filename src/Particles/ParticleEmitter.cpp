#include "ParticleEmitter.h"
#include "TextureManager.h"
#include <algorithm>
#include <assert.h>
#include <string>
#include <random>

inline float Deg2Rad(float a)
{
	return a * 0.01745329252f;
}

inline float Rad2Deg(float a)
{
	return a * 57.29577951f;
}

inline float clampf(float value, float min_inclusive, float max_inclusive)
{
	if (min_inclusive > max_inclusive)
	{
		std::swap(min_inclusive, max_inclusive);
	}
	return value < min_inclusive ? min_inclusive : value < max_inclusive ? value : max_inclusive;
}

inline void normalize_point(float x, float y, particles::Point* out)
{
	float n = x * x + y * y;
	if (n == 1.0f)
	{
		return;
	}

	n = sqrt(n);
	if (n < 1e-5)
	{
		return;
	}

	n = 1.0f / n;
	out->x = x * n;
	out->y = y * n;
}

inline static float RANDOM_M11()
{
	thread_local std::mt19937 rng(std::random_device{}());

	std::uniform_int_distribution<unsigned int> seedDist(std::numeric_limits<unsigned int>::min(),
		std::numeric_limits<unsigned int>::max());

	unsigned int seed = seedDist(rng);

	seed = seed * 134775813 + 1;
	union
	{
		uint32_t d;
		float f;
	} u;
	u.d = (((uint32_t)(seed) & 0x7fff) << 8) | 0x40000000;
	return u.f - 3.0f;
}

particles::ParticleEmitter::ParticleEmitter()
{
}

// ParticleSystem
bool particles::ParticleEmitter::initWithTotalParticles(int numberOfParticles)
{
	_totalParticles = numberOfParticles;
	_isActive = true;
	_emitterMode = eMode::GRAVITY;
	_isAutoRemoveOnFinish = false;

	resetTotalParticles(numberOfParticles);

	return true;
}

void particles::ParticleEmitter::resetTotalParticles(int numberOfParticles)
{
	if (particle_data_.size() < numberOfParticles)
		particle_data_.resize(numberOfParticles);
}

particles::ParticleEmitter::~ParticleEmitter()
{
	SDL_DestroyTexture(_texture);
}

void particles::ParticleEmitter::addParticles(int count)
{
	if (_paused) return;

	int start = _particleCount;
	_particleCount += count;

	for (int i = start; i < _particleCount; ++i)
	{
		float theLife = _life + _lifeVar * RANDOM_M11();
		particle_data_[i].timeToLive = (std::max)(0.0f, theLife);
	}

	for (int i = start; i < _particleCount; ++i)
	{
		particle_data_[i].posx = _sourcePosition.x + _posVar.x * RANDOM_M11();
	}

	for (int i = start; i < _particleCount; ++i)
	{
		particle_data_[i].posy = _sourcePosition.y + _posVar.y * RANDOM_M11();
	}

#define SET_COLOR(c, b, v)																					\
	for (int i = start; i < _particleCount; ++i)															\
		particle_data_[i].c = clampf(b + v * RANDOM_M11(), 0, 1);\

	SET_COLOR(colorR, _startColor.r, _startColorVar.r);
	SET_COLOR(colorG, _startColor.g, _startColorVar.g);
	SET_COLOR(colorB, _startColor.b, _startColorVar.b);
	SET_COLOR(colorA, _startColor.a, _startColorVar.a);

	SET_COLOR(deltaColorR, _endColor.r, _endColorVar.r);
	SET_COLOR(deltaColorG, _endColor.g, _endColorVar.g);
	SET_COLOR(deltaColorB, _endColor.b, _endColorVar.b);
	SET_COLOR(deltaColorA, _endColor.a, _endColorVar.a);

#define SET_DELTA_COLOR(c, dc)																				\
	for (int i = start; i < _particleCount; ++i)															\
		particle_data_[i].dc = (particle_data_[i].dc - particle_data_[i].c) / particle_data_[i].timeToLive;	\

	SET_DELTA_COLOR(colorR, deltaColorR);
	SET_DELTA_COLOR(colorG, deltaColorG);
	SET_DELTA_COLOR(colorB, deltaColorB);
	SET_DELTA_COLOR(colorA, deltaColorA);

	//size
	for (int i = start; i < _particleCount; ++i)
	{
		particle_data_[i].size = _startSize + _startSizeVar * RANDOM_M11();
		particle_data_[i].size = (std::max)(0.0f, particle_data_[i].size);
	}

	if (static_cast<int>(_endSize) != static_cast<int>(START_SIZE_EQUAL_TO_END_SIZE))
	{
		for (int i = start; i < _particleCount; ++i)
		{
			float endSize = _endSize + _endSizeVar * RANDOM_M11();
			endSize = (std::max)(0.0f, endSize);
			particle_data_[i].deltaSize = (endSize - particle_data_[i].size) / particle_data_[i].timeToLive;
		}
	}
	else
	{
		for (int i = start; i < _particleCount; ++i)
		{
			particle_data_[i].deltaSize = 0.0f;
		}
	}

	// rotation
	for (int i = start; i < _particleCount; ++i)
	{
		particle_data_[i].rotation = _startSpin + _startSpinVar * RANDOM_M11();
	}
	for (int i = start; i < _particleCount; ++i)
	{
		float endA = _endSpin + _endSpinVar * RANDOM_M11();
		particle_data_[i].deltaRotation = (endA - particle_data_[i].rotation) / particle_data_[i].timeToLive;
	}

	// position
	Vec2 pos;
	pos.x = static_cast<float>(x_);
	pos.y = static_cast<float>(y_);

	for (int i = start; i < _particleCount; ++i)
	{
		particle_data_[i].startPosX = pos.x;
	}
	for (int i = start; i < _particleCount; ++i)
	{
		particle_data_[i].startPosY = pos.y;
	}

	// Mode Gravity: A
	if (_emitterMode == eMode::GRAVITY)
	{

		// radial accel
		for (int i = start; i < _particleCount; ++i)
		{
			particle_data_[i].modeA.radialAccel = modeA.radialAccel + modeA.radialAccelVar * RANDOM_M11();
		}

		// tangential accel
		for (int i = start; i < _particleCount; ++i)
		{
			particle_data_[i].modeA.tangentialAccel = modeA.tangentialAccel + modeA.tangentialAccelVar * RANDOM_M11();
		}

		// rotation is dir
		if (modeA.rotationIsDir)
		{
			for (int i = start; i < _particleCount; ++i)
			{
				float a = Deg2Rad(_angle + _angleVar * RANDOM_M11());
				Vec2 v(cosf(a), sinf(a));
				float s = modeA.speed + modeA.speedVar * RANDOM_M11();
				Vec2 dir = v * s;
				particle_data_[i].modeA.dirX = dir.x;    //v * s ;
				particle_data_[i].modeA.dirY = dir.y;
				particle_data_[i].rotation = -Rad2Deg(dir.getAngle());
			}
		}
		else
		{
			for (int i = start; i < _particleCount; ++i)
			{
				float a = Deg2Rad(_angle + _angleVar * RANDOM_M11());
				Vec2 v(cosf(a), sinf(a));
				float s = modeA.speed + modeA.speedVar * RANDOM_M11();
				Vec2 dir = v * s;
				particle_data_[i].modeA.dirX = dir.x;    //v * s ;
				particle_data_[i].modeA.dirY = dir.y;
			}
		}
	}

	// Mode Radius: B
	else
	{
		for (int i = start; i < _particleCount; ++i)
		{
			particle_data_[i].modeB.radius = modeB.startRadius + modeB.startRadiusVar * RANDOM_M11();
		}

		for (int i = start; i < _particleCount; ++i)
		{
			particle_data_[i].modeB.angle = Deg2Rad(_angle + _angleVar * RANDOM_M11());
		}

		for (int i = start; i < _particleCount; ++i)
		{
			particle_data_[i].modeB.degreesPerSecond = Deg2Rad(modeB.rotatePerSecond + modeB.rotatePerSecondVar * RANDOM_M11());
		}

		if (static_cast<int>(modeB.endRadius) == static_cast<int>(START_RADIUS_EQUAL_TO_END_RADIUS))
		{
			for (int i = start; i < _particleCount; ++i)
			{
				particle_data_[i].modeB.deltaRadius = 0.0f;
			}
		}
		else
		{
			for (int i = start; i < _particleCount; ++i)
			{
				float endRadius = modeB.endRadius + modeB.endRadiusVar * RANDOM_M11();
				particle_data_[i].modeB.deltaRadius = (endRadius - particle_data_[i].modeB.radius) / particle_data_[i].timeToLive;
			}
		}
	}
}

void particles::ParticleEmitter::stopSystem()
{
	_isActive = false;
	_elapsed = _duration;
	_emitCounter = 0;
}

void particles::ParticleEmitter::resetSystem()
{
	_isActive = true;
	_elapsed = 0;
	for (int i = 0; i < _particleCount; ++i)
	{
		particle_data_[i].timeToLive = 0.0f;
	}
}

bool particles::ParticleEmitter::isFull()
{
	return (_particleCount == _totalParticles);
}

// ParticleSystem - MainLoop
void particles::ParticleEmitter::update()
{
	float dt = 1.f / 25;
	if (_isActive && _emissionRate)
	{
		float rate = 1.0f / _emissionRate;
		int totalParticles = _totalParticles;

		if (_particleCount < totalParticles)
		{
			_emitCounter += dt;
			if (_emitCounter < 0.f)
			{
				_emitCounter = 0.f;
			}
		}

		int emitCount = static_cast<int>((std::min)(1.0f * (totalParticles - _particleCount), _emitCounter / rate));
		addParticles(emitCount);
		_emitCounter -= rate * emitCount;

		_elapsed += dt;
		if (_elapsed < 0.f)
		{
			_elapsed = 0.f;
		}
		if (static_cast<int>(_duration) != static_cast<int>(DURATION_INFINITY) && _duration < _elapsed)
		{
			this->stopSystem();
		}
	}

	for (int i = 0; i < _particleCount; ++i)
	{
		particle_data_[i].timeToLive -= dt;
	}

	// rebirth
	for (int i = 0; i < _particleCount; ++i)
	{
		if (particle_data_[i].timeToLive <= 0.0f)
		{
			int j = _particleCount - 1;
			particle_data_[i] = particle_data_[_particleCount - 1];
			--_particleCount;
		}
	}

	if (_emitterMode == eMode::GRAVITY)
	{
		for (int i = 0; i < _particleCount; ++i)
		{
			Point tmp, radial = { 0.0f, 0.0f }, tangential;

			if (particle_data_[i].posx || particle_data_[i].posy)
			{
				normalize_point(particle_data_[i].posx, particle_data_[i].posy, &radial);
			}
			tangential = radial;
			radial.x *= particle_data_[i].modeA.radialAccel;
			radial.y *= particle_data_[i].modeA.radialAccel;

			std::swap(tangential.x, tangential.y);
			tangential.x *= -particle_data_[i].modeA.tangentialAccel;
			tangential.y *= particle_data_[i].modeA.tangentialAccel;

			tmp.x = radial.x + tangential.x + modeA.gravity.x;
			tmp.y = radial.y + tangential.y + modeA.gravity.y;
			tmp.x *= dt;
			tmp.y *= dt;

			particle_data_[i].modeA.dirX += tmp.x;
			particle_data_[i].modeA.dirY += tmp.y;

			tmp.x = particle_data_[i].modeA.dirX * dt * _yCoordFlipped;
			tmp.y = particle_data_[i].modeA.dirY * dt * _yCoordFlipped;
			particle_data_[i].posx += tmp.x;
			particle_data_[i].posy += tmp.y;
		}
	}
	else
	{
		for (int i = 0; i < _particleCount; ++i)
		{
			particle_data_[i].modeB.angle += particle_data_[i].modeB.degreesPerSecond * dt;
			particle_data_[i].modeB.radius += particle_data_[i].modeB.deltaRadius * dt;
			particle_data_[i].posx = -cosf(particle_data_[i].modeB.angle) * particle_data_[i].modeB.radius;
			particle_data_[i].posy = -sinf(particle_data_[i].modeB.angle) * particle_data_[i].modeB.radius * _yCoordFlipped;
		}
	}

	//color, size, rotation
	for (int i = 0; i < _particleCount; ++i)
	{
		particle_data_[i].colorR += particle_data_[i].deltaColorR * dt;
		particle_data_[i].colorG += particle_data_[i].deltaColorG * dt;
		particle_data_[i].colorB += particle_data_[i].deltaColorB * dt;
		particle_data_[i].colorA += particle_data_[i].deltaColorA * dt;
		particle_data_[i].size += (particle_data_[i].deltaSize * dt);
		particle_data_[i].size = (std::max)(0.0f, particle_data_[i].size);
		particle_data_[i].rotation += particle_data_[i].deltaRotation * dt;
	}
}

// ParticleSystem - Texture protocol
void particles::ParticleEmitter::setTexture(SDL_Texture* var)
{
	if (_texture != var)
	{
		//SDL_DestroyTexture(_texture);
		_texture = var;
	}
}

void particles::ParticleEmitter::draw()
{
	if (_texture == nullptr)
	{
		return;
	}
	for (int i = 0; i < _particleCount; i++)
	{
		auto& p = particle_data_[i];
		if (p.size <= 0 || p.colorA <= 0 || p.timeToLive <= 0)
		{
			continue;
		}
		SDL_Rect r = { int(p.posx + p.startPosX - p.size / 2), int(p.posy + p.startPosY - p.size / 2), int(p.size), int(p.size) };
		SDL_Color c = { Uint8(p.colorR * 255), Uint8(p.colorG * 255), Uint8(p.colorB * 255), Uint8(p.colorA * 255) };
		SDL_SetTextureColorMod(_texture, c.r, c.g, c.b);
		SDL_SetTextureAlphaMod(_texture, c.a);
		SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
		TextureManager::draw(_texture, nullptr, &r, p.rotation, SDL_FLIP_NONE);
	}
}

SDL_Texture* particles::ParticleEmitter::getTexture()
{
	return _texture;
}

// ParticleEmitter - Properties of Gravity Mode
void particles::ParticleEmitter::setTangentialAccel(float t)
{
	modeA.tangentialAccel = t;
}

float particles::ParticleEmitter::getTangentialAccel() const
{
	return modeA.tangentialAccel;
}

void particles::ParticleEmitter::setTangentialAccelVar(float t)
{
	modeA.tangentialAccelVar = t;
}

float particles::ParticleEmitter::getTangentialAccelVar() const
{
	return modeA.tangentialAccelVar;
}

void particles::ParticleEmitter::setRadialAccel(float t)
{
	modeA.radialAccel = t;
}

float particles::ParticleEmitter::getRadialAccel() const
{
	return modeA.radialAccel;
}

void particles::ParticleEmitter::setRadialAccelVar(float t)
{
	modeA.radialAccelVar = t;
}

float particles::ParticleEmitter::getRadialAccelVar() const
{
	return modeA.radialAccelVar;
}

void particles::ParticleEmitter::setRotationIsDir(bool t)
{
	modeA.rotationIsDir = t;
}

bool particles::ParticleEmitter::getRotationIsDir() const
{
	return modeA.rotationIsDir;
}

void particles::ParticleEmitter::setGravity(const Vec2& g)
{
	modeA.gravity = g;
}

const particles::Vec2& particles::ParticleEmitter::getGravity()
{
	return modeA.gravity;
}

void particles::ParticleEmitter::setSpeed(float speed)
{
	modeA.speed = speed;
}

float particles::ParticleEmitter::getSpeed() const
{
	return modeA.speed;
}

void particles::ParticleEmitter::setSpeedVar(float speedVar)
{

	modeA.speedVar = speedVar;
}

float particles::ParticleEmitter::getSpeedVar() const
{

	return modeA.speedVar;
}

// ParticleSystem - Properties of Radius Mode
void particles::ParticleEmitter::setStartRadius(float startRadius)
{
	modeB.startRadius = startRadius;
}

float particles::ParticleEmitter::getStartRadius() const
{
	return modeB.startRadius;
}

void particles::ParticleEmitter::setStartRadiusVar(float startRadiusVar)
{
	modeB.startRadiusVar = startRadiusVar;
}

float particles::ParticleEmitter::getStartRadiusVar() const
{
	return modeB.startRadiusVar;
}

void particles::ParticleEmitter::setEndRadius(float endRadius)
{
	modeB.endRadius = endRadius;
}

float particles::ParticleEmitter::getEndRadius() const
{
	return modeB.endRadius;
}

void particles::ParticleEmitter::setEndRadiusVar(float endRadiusVar)
{
	modeB.endRadiusVar = endRadiusVar;
}

float particles::ParticleEmitter::getEndRadiusVar() const
{

	return modeB.endRadiusVar;
}

void particles::ParticleEmitter::setRotatePerSecond(float degrees)
{
	modeB.rotatePerSecond = degrees;
}

float particles::ParticleEmitter::getRotatePerSecond() const
{
	return modeB.rotatePerSecond;
}

void particles::ParticleEmitter::setRotatePerSecondVar(float degrees)
{
	modeB.rotatePerSecondVar = degrees;
}

float particles::ParticleEmitter::getRotatePerSecondVar() const
{
	return modeB.rotatePerSecondVar;
}

bool particles::ParticleEmitter::isActive() const
{
	return _isActive;
}

int particles::ParticleEmitter::getTotalParticles() const
{
	return _totalParticles;
}

void particles::ParticleEmitter::setTotalParticles(int var)
{
	_totalParticles = var;
}

bool particles::ParticleEmitter::isAutoRemoveOnFinish() const
{
	return _isAutoRemoveOnFinish;
}

void particles::ParticleEmitter::setAutoRemoveOnFinish(bool var)
{
	_isAutoRemoveOnFinish = var;
}

bool particles::ParticleEmitter::isPaused() const
{
	return _paused;
}

void particles::ParticleEmitter::pauseEmissions()
{
	_paused = true;
}

void particles::ParticleEmitter::resumeEmissions()
{
	_paused = false;
}