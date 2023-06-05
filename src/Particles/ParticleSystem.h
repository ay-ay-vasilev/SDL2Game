#pragma once

#include "SDL.h"
#include <vector>
#include <string>

struct Point
{
public:
	Point() {}
	Point(float x, float y) : x(x), y(y) {}

	~Point() {}
	float x = 0, y = 0;
	Point operator*(float f)
	{
		Point p{ x * f, y * f };
		return p;
	}
	float getAngle()
	{
		return atan2f(y, x);
	}
};

typedef Point Vec2;

struct Color4F
{
	float r = 0, g = 0, b = 0, a = 0;
};

class ParticleData
{
public:
	float posx = 0;
	float posy = 0;
	float startPosX = 0;
	float startPosY = 0;

	float colorR = 0;
	float colorG = 0;
	float colorB = 0;
	float colorA = 0;

	float deltaColorR = 0;
	float deltaColorG = 0;
	float deltaColorB = 0;
	float deltaColorA = 0;

	float size = 0;
	float deltaSize = 0;
	float rotation = 0;
	float deltaRotation = 0;
	float timeToLive = 0;
	unsigned int atlasIndex = 0;

	//! Mode A: gravity, direction, radial accel, tangential accel
	struct ModeA
	{
		float dirX = 0;
		float dirY = 0;
		float radialAccel = 0;
		float tangentialAccel = 0;
	};

	//! Mode B: radius mode
	struct ModeB
	{
		float angle = 0;
		float degreesPerSecond = 0;
		float radius = 0;
		float deltaRadius = 0;
	};

	ModeA modeA;
	ModeB modeB;
};

class ParticleSystem
{
public:
	enum class eMode
	{
		GRAVITY,
		RADIUS,
	};

	enum
	{
		DURATION_INFINITY = -1,
		START_SIZE_EQUAL_TO_END_SIZE = -1,
		START_RADIUS_EQUAL_TO_END_RADIUS = -1,
	};

public:
	void addParticles(int count);

	void stopSystem();
	void resetSystem();
	bool isFull();

	virtual bool isAutoRemoveOnFinish() const;
	virtual void setAutoRemoveOnFinish(bool var);

	// Mode A
	virtual const Vec2& getGravity();
	virtual void setGravity(const Vec2& g);

	virtual float getSpeed() const;
	virtual void setSpeed(float speed);
	virtual float getSpeedVar() const;
	virtual void setSpeedVar(float speed);

	virtual float getTangentialAccel() const;
	virtual void setTangentialAccel(float t);
	virtual float getTangentialAccelVar() const;
	virtual void setTangentialAccelVar(float t);

	virtual float getRadialAccel() const;
	virtual void setRadialAccel(float t);
	virtual float getRadialAccelVar() const;
	virtual void setRadialAccelVar(float t);

	virtual bool getRotationIsDir() const;
	virtual void setRotationIsDir(bool t);

	// Mode B
	virtual float getStartRadius() const;
	virtual void setStartRadius(float startRadius);
	virtual float getStartRadiusVar() const;
	virtual void setStartRadiusVar(float startRadiusVar);

	virtual float getEndRadius() const;
	virtual void setEndRadius(float endRadius);
	virtual float getEndRadiusVar() const;
	virtual void setEndRadiusVar(float endRadiusVar);

	virtual float getRotatePerSecond() const;
	virtual void setRotatePerSecond(float degrees);
	virtual float getRotatePerSecondVar() const;
	virtual void setRotatePerSecondVar(float degrees);

	virtual bool isActive() const;

	unsigned int getParticleCount() const { return _particleCount; }
	float getDuration() const { return _duration; }
	void setDuration(float duration) { _duration = duration; }

	const Vec2& getSourcePosition() const { return _sourcePosition; }
	void setSourcePosition(const Vec2& pos) { _sourcePosition = pos; }

	const Vec2& getPosVar() const { return _posVar; }
	void setPosVar(const Vec2& pos) { _posVar = pos; }

	float getLife() const { return _life; }
	void setLife(float life) { _life = life; }
	float getLifeVar() const { return _lifeVar; }
	void setLifeVar(float lifeVar) { _lifeVar = lifeVar; }

	float getAngle() const { return _angle; }
	void setAngle(float angle) { _angle = angle; }
	float getAngleVar() const { return _angleVar; }
	void setAngleVar(float angleVar) { _angleVar = angleVar; }

	eMode getEmitterMode() const { return _emitterMode; }
	void setEmitterMode(eMode mode) { _emitterMode = mode; }

	float getStartSize() const { return _startSize; }
	void setStartSize(float startSize) { _startSize = startSize; }
	float getStartSizeVar() const { return _startSizeVar; }
	void setStartSizeVar(float sizeVar) { _startSizeVar = sizeVar; }

	float getEndSize() const { return _endSize; }
	void setEndSize(float endSize) { _endSize = endSize; }
	float getEndSizeVar() const { return _endSizeVar; }
	void setEndSizeVar(float sizeVar) { _endSizeVar = sizeVar; }

	const Color4F& getStartColor() const { return _startColor; }
	void setStartColor(const Color4F& color) { _startColor = color; }
	const Color4F& getStartColorVar() const { return _startColorVar; }
	void setStartColorVar(const Color4F& color) { _startColorVar = color; }

	const Color4F& getEndColor() const { return _endColor; }
	void setEndColor(const Color4F& color) { _endColor = color; }
	const Color4F& getEndColorVar() const { return _endColorVar; }
	void setEndColorVar(const Color4F& color) { _endColorVar = color; }

	float getStartSpin() const { return _startSpin; }
	void setStartSpin(float spin) { _startSpin = spin; }
	float getStartSpinVar() const { return _startSpinVar; }
	void setStartSpinVar(float pinVar) { _startSpinVar = pinVar; }

	float getEndSpin() const { return _endSpin; }
	void setEndSpin(float endSpin) { _endSpin = endSpin; }
	float getEndSpinVar() const { return _endSpinVar; }
	void setEndSpinVar(float endSpinVar) { _endSpinVar = endSpinVar; }

	float getEmissionRate() const { return _emissionRate; }
	void setEmissionRate(float rate) { _emissionRate = rate; }

	virtual int getTotalParticles() const;
	virtual void setTotalParticles(int totalParticles);

	SDL_Texture* getTexture();
	void setTexture(SDL_Texture* texture);
	void draw();
	void update();

	ParticleSystem();
	virtual ~ParticleSystem();

	virtual bool initWithTotalParticles(int numberOfParticles);

	virtual void resetTotalParticles(int numberOfParticles);
	virtual bool isPaused() const;
	virtual void pauseEmissions();
	virtual void resumeEmissions();

protected:
	bool _isAutoRemoveOnFinish = false;
	float _elapsed = 0;

	// Different modes
	struct ModeA
	{
		Vec2 gravity;
		float speed = 0;
		float speedVar = 0;
		float tangentialAccel = 0;
		float tangentialAccelVar = 0;
		float radialAccel = 0;
		float radialAccelVar = 0;
		bool rotationIsDir = 0;
	};

	struct ModeB
	{
		float startRadius = 0;
		float startRadiusVar = 0;
		float endRadius = 0;
		float endRadiusVar = 0;
		float rotatePerSecond = 0;
		float rotatePerSecondVar = 0;
	};

	ModeA modeA;
	ModeB modeB;

	//particle data
	std::vector<ParticleData> particle_data_;
	//! How many particles can be emitted per second
	float _emitCounter = 0;

	bool _isActive = true;
	int _particleCount = 0;
	/** How many seconds the emitter will run. -1 means 'forever' */
	float _duration = 0;

	Vec2 _sourcePosition;
	Vec2 _posVar;

	float _life = 0;
	float _lifeVar = 0;

	float _angle = 0;
	float _angleVar = 0;

	eMode _emitterMode = eMode::GRAVITY;

	float _startSize = 0;
	float _startSizeVar = 0;

	float _endSize = 0;
	float _endSizeVar = 0;

	Color4F _startColor;
	Color4F _startColorVar;

	Color4F _endColor;
	Color4F _endColorVar;

	float _startSpin = 0;
	float _startSpinVar = 0;

	float _endSpin = 0;
	float _endSpinVar = 0;

	float _emissionRate = 0;
	int _totalParticles = 0;

	SDL_Texture* _texture = nullptr;
	/** does FlippedY variance of each particle */
	int _yCoordFlipped = 1;

	bool _paused = false;
	int x_ = 0, y_ = 0;
public:
	void setPosition(int x, int y) { x_ = x; y_ = y; }
};