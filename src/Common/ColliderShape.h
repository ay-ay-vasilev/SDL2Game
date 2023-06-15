#pragma once
#include "Vector2D.h"
#include "Collision.h"

class ColliderShape
{
public:
	ColliderShape() = default;
	virtual ~ColliderShape() = default;
	virtual bool collidesWith(const std::shared_ptr<ColliderShape>& other) const = 0;
	virtual float getOverlapX(const std::shared_ptr<ColliderShape>& other) const = 0;
	virtual float getOverlapY(const std::shared_ptr<ColliderShape>& other) const = 0;
	virtual void setPosition(const Vector2D& position) = 0;
	virtual void movePosition(const Vector2D& positionDelta) = 0;
	virtual Vector2D getPosition() const = 0;
	virtual SDL_Rect getDrawRect() const = 0;
	virtual void setScale(float scaleVal) = 0;
	virtual float getWidth() const = 0;
	virtual float getHeight() const = 0;
	virtual float getLowestPoint() const = 0;
};

class RectangleCollider : public ColliderShape
{
public:
	RectangleCollider(const Vector2D& position, float width, float height) :
		position({ position.x - width/2, position.y - height/2 }), w(width), h(height) {}

	bool collidesWith(const std::shared_ptr<ColliderShape>& other) const override;
	float getOverlapX(const std::shared_ptr<ColliderShape>& other) const override;
	float getOverlapY(const std::shared_ptr<ColliderShape>& other) const override;

	void inline setPosition(const Vector2D& newPosition) override { position = { newPosition.x - w / 2, newPosition.y - h / 2 }; }
	void inline movePosition(const Vector2D& positionDelta) override { position += positionDelta; }
	Vector2D inline getPosition() const override { return { position.x, position.y }; }
	SDL_Rect inline getDrawRect() const override { return { static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(w), static_cast<int>(h) }; }
	void inline setScale(float scaleVal) override
	{
		const Vector2D originalPosition{ position.x + w / 2, position.y + h / 2 };
		w *= scaleVal; h *= scaleVal;
		setPosition({ originalPosition.x - w / 2, originalPosition.y - h / 2 });
	}
	float inline getWidth() const override { return w; }
	float inline getHeight() const override { return h; }
	float inline getLowestPoint() const override { return position.y + w / 2; }
private:
	Vector2D position;
	float w, h;
};

class CircleCollider : public ColliderShape {
public:
	CircleCollider(const Vector2D& position, float radius) :
		position(position), radius(radius) {}

	bool collidesWith(const std::shared_ptr<ColliderShape>& other) const;
	float getOverlapX(const std::shared_ptr<ColliderShape>& other) const;
	float getOverlapY(const std::shared_ptr<ColliderShape>& other) const;

	void inline setPosition(const Vector2D& newPosition) override { position = newPosition; }
	void inline movePosition(const Vector2D& positionDelta) override { position += positionDelta; }
	Vector2D inline getPosition() const override { return position; }
	SDL_Rect inline getDrawRect() const override { return { static_cast<int>(position.x - radius), static_cast<int>(position.y - radius), static_cast<int>(radius) * 2, static_cast<int>(radius) * 2 }; }
	void inline setScale(float scaleVal) override { radius *= scaleVal; }
	float inline getWidth() const override { return radius * 2; }
	float inline getHeight() const override { return radius * 2; }
	float inline getRadius() const { return radius; }
	float inline getLowestPoint() const override { return position.y + radius; }

private:
	Vector2D position;
	float radius;
};