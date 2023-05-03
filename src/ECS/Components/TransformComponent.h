#pragma once
#include "ECS.h"
#include "Vector2D.h"

#include "Game.h"

namespace ecs
{
	class TransformComponent : public Component
	{
	public:
		explicit TransformComponent() : position() {}
		TransformComponent(const float x, const float y) : position(x, y) {}
		TransformComponent(const float x, const float y, const int w, const int h, const float scale, const float speed = 0) :
			position(x, y), width(w), height(h), scale(scale), speed(speed) {}

		void init() override
		{
			velocity.Zero();
			speed *= scale;
		}
		void update(double delta) override
		{
			velocity.Normalize();
			direction.Normalize();

			position.x += velocity.x * speed * delta * Game::constants->TIME_SCALE;
			position.y += velocity.y * speed * delta * Game::constants->TIME_SCALE;
		}

		const inline Vector2D getPosition() const { return position; }
		const inline Vector2D getVelocity() const { return velocity; }
		const inline Vector2D getDirection() const { return direction; }

		void inline setVeloctiy(float x, float y) { velocity.x = x; velocity.y = y; }
		void inline setVeloctiy(const Vector2D& newVelocity) { velocity = newVelocity; }
		void inline setPosition(float x, float y) { position.x = x; position.y = y; }
		void inline setPosition(const Vector2D& newPosition) { position = newPosition; }
		void inline movePosition(float dx, float dy) { position.x += dx; position.y += dy; }
		void inline movePosition(const Vector2D& positionDelta) { position += positionDelta; }
		void inline setDirection(float x, float y)
		{
			direction.x = x; direction.y = y;
			direction.Normalize();
		}
		void inline setDirection(const Vector2D& newDirection)
		{
			direction = newDirection;
			direction.Normalize();
		}
		void inline setDirectionX(float x) { direction.x = x; }
		void inline setDirectionY(float y) { direction.y = y; }

		const int inline getWidth() const { return width; }
		const int inline getHeight() const { return height; }
		const float inline getScale() const { return scale; }

		const float inline getSpeed() const{ return speed; }
		void inline setSpeed(float newSpeed) { speed = newSpeed; }

	private:
		Vector2D position;
		Vector2D velocity;
		Vector2D direction{ 1, 0 };

		int width = 0;
		int height = 0;
		float scale = 0;

		float speed = 0;
	};
}