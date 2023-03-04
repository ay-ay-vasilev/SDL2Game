#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:
	TransformComponent() : position() {}
	TransformComponent(const float x, const float y) : position(x, y) {}
	TransformComponent(const float x, const float y, const int w, const int h, const float scale, const int speed = 0) :
		position(x, y), width(w), height(h), scale(scale), speed(speed) {}

	void init() override
	{
		velocity.Zero();
	}
	void update() override
	{
		velocity.Normalize();
		direction.Normalize();

		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

	const Vector2D getPosition() const { return position; }
	const Vector2D getVelocity() const { return velocity; }
	const Vector2D getDirection() const { return direction; }

	void setVeloctiy(float x, float y) { velocity.x = x; velocity.y = y; }
	void setVeloctiy(Vector2D newVelocity) { velocity = newVelocity; }
	void setPosition(float x, float y) { position.x = x; position.y = y; }
	void setPosition(Vector2D newPosition) { position = newPosition; }
	void setDirection(float x, float y) { direction.x = x; direction.y = y; }
	void setDirection(Vector2D newDirection) { direction = newDirection; }
	void setDirectionX(float x) { direction.x = x; }
	void setDirectionY(float y) { direction.y = y; }

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	float getScale() const { return scale; }

	void setSpeed(int newSpeed) { speed = newSpeed; }

private:
	Vector2D position;
	Vector2D velocity;
	Vector2D direction{ 1, 0 };

	int width = 0;
	int height = 0;
	float scale = 0;

	int speed = 0;

};