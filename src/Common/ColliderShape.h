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
	virtual Vector2D getPosition() const = 0;
	virtual void setScale(float scaleVal) = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
};

class RectangleCollider : public ColliderShape
{
public:
	RectangleCollider(const Vector2D& position, float width, float height) :
		position(position), w(width), h(height) {}

	bool collidesWith(const ColliderShape& other) const override;
	float getOverlapX(const ColliderShape& other) const override;
	float getOverlapY(const ColliderShape& other) const override;

	void setPosition(const Vector2D& newPosition) override { position = newPosition; }
	Vector2D getPosition() const override { return position; }
	void setScale(float scaleVal) override { w *= scaleVal; h *= scaleVal; }
	int getWidth() const override { return w; }
	int getHeight() const override { return h; }
private:
	Vector2D position;
	int w, h;
};

class CircleCollider : public ColliderShape {
public:
	CircleCollider(const Vector2D& position, float radius) :
		position(position), radius(radius) {}

	bool collidesWith(const ColliderShape& other) const;
	float getOverlapX(const ColliderShape& other) const;
	float getOverlapY(const ColliderShape& other) const;

	void setPosition(const Vector2D& newPosition) override { position = newPosition; }
	Vector2D getPosition() const override { return position; }
	void setScale(float scaleVal) override { radius *= scaleVal; }
	int getWidth() const override { return radius * 2; }
	int getHeight() const override { return radius * 2; }

	float getRadius() const { return radius; }

private:
	Vector2D position;
	int radius;
};