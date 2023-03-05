#pragma once
#include "Vector2D.h"
#include "Collision.h"

class ColliderShape
{
public:
	ColliderShape() = default;
	virtual ~ColliderShape() = default;
	virtual bool collidesWith(const ColliderShape& other) const = 0;
	virtual float getOverlapX(const ColliderShape& other) const = 0;
	virtual float getOverlapY(const ColliderShape& other) const = 0;
	virtual void setPosition(const Vector2D& position) = 0;
	virtual void movePosition(const Vector2D& positionDelta) = 0;
	virtual Vector2D getPosition() const = 0;
	virtual SDL_Rect getDrawRect() const = 0;
	virtual void setScale(float scaleVal) = 0;
	virtual float getWidth() const = 0;
	virtual float getHeight() const = 0;
};

class RectangleCollider : public ColliderShape
{
public:
	RectangleCollider(const Vector2D& position, float width, float height) :
		position({ position.x - width/2, position.y - height/2 }), w(width), h(height) {}

	bool collidesWith(const ColliderShape& other) const override;
	float getOverlapX(const ColliderShape& other) const override;
	float getOverlapY(const ColliderShape& other) const override;

	void setPosition(const Vector2D& newPosition) override { position = { newPosition.x - w / 2, newPosition.y - h / 2 }; }
	void movePosition(const Vector2D& positionDelta) override { position += positionDelta; }
	Vector2D getPosition() const override { return { position.x, position.y }; }
	SDL_Rect getDrawRect() const override { return { static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(w), static_cast<int>(h) }; }
	void setScale(float scaleVal) override
	{
		const Vector2D originalPosition{ position.x + w / 2, position.y + h / 2 };
		w *= scaleVal; h *= scaleVal;
		setPosition({ originalPosition.x - w / 2, originalPosition.y - h / 2 });
	}
	float getWidth() const override { return w; }
	float getHeight() const override { return h; }
private:
	Vector2D position;
	float w, h;
};

class CircleCollider : public ColliderShape {
public:
	CircleCollider(const Vector2D& position, float radius) :
		position(position), radius(radius) {}

	bool collidesWith(const ColliderShape& other) const;
	float getOverlapX(const ColliderShape& other) const;
	float getOverlapY(const ColliderShape& other) const;

	void setPosition(const Vector2D& newPosition) override { position = newPosition; }
	void movePosition(const Vector2D& positionDelta) override { position += positionDelta; }
	Vector2D getPosition() const override { return position; }
	SDL_Rect getDrawRect() const override { return { static_cast<int>(position.x - radius), static_cast<int>(position.y - radius), static_cast<int>(radius) * 2, static_cast<int>(radius) * 2 }; }
	void setScale(float scaleVal) override { radius *= scaleVal; }
	float getWidth() const override { return radius * 2; }
	float getHeight() const override { return radius * 2; }

	float getRadius() const { return radius; }

private:
	Vector2D position;
	float radius;
};