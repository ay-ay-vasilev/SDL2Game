#pragma once
#include "Components.h"
#include "TextureManager.h"
#include "ColliderShape.h"

#include <SDL.h>
#include <json.hpp>

#include <string>

class ColliderComponent : public Component
{
public:
	ColliderComponent(const std::string_view& tag) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider(),
		colliderOffset() {};

	ColliderComponent(const std::string_view& tag, const Vector2D& position, const float radius, const Vector2D& colliderOffset = Vector2D()) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider(std::make_shared<CircleCollider>(position, radius)),
		colliderOffset(colliderOffset) {};

	ColliderComponent(const std::string_view& tag, const Vector2D& position, const int width, const int height, const Vector2D& colliderOffset = Vector2D()) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider(std::make_shared<RectangleCollider>(position, width, height)),
		colliderOffset(colliderOffset) {};

	ColliderComponent(const std::string_view& tag, const std::shared_ptr<ColliderShape>& colliderShape, const Vector2D& colliderOffset = Vector2D()) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider(colliderShape),
		colliderOffset(colliderOffset) {};

	ColliderComponent(const std::string_view& tag, const nlohmann::json& colliderData) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		colliderOffset({ colliderData["dx"], colliderData["dy"] })
	{
		if (colliderData["shape"] == "circle")
		{
			collider = std::make_shared<CircleCollider>(Vector2D(0, 0), colliderData["radius"]);
		}
		if (colliderData["shape"] == "rectangle")
		{
			collider = std::make_shared<RectangleCollider>(Vector2D(0, 0), colliderData["w"], colliderData["h"]);
		}
	}
	
	void init() override
	{
		if (entity->hasComponent<TransformComponent>())
		{
			transform = &entity->getComponent<TransformComponent>();
			colliderOffset.x *= transform->getScale();
			colliderOffset.y *= transform->getScale();

			collider->setScale(transform->getScale());
			const auto colliderPosition = collider->getPosition();
			const auto colliderX = colliderPosition.x + transform->getPosition().x + colliderOffset.x - (collider->getWidth()) / 2;
			const auto colliderY = colliderPosition.y + transform->getPosition().y + colliderOffset.y - (collider->getHeight()) / 2;
			collider->setPosition(Vector2D(colliderX, colliderY));
		}

		destRect = { static_cast<int>(collider->getPosition().x), static_cast<int>(collider->getPosition().y), collider->getWidth(), collider->getHeight()};

		std::string texturePath;
		if (auto rectCollider = std::dynamic_pointer_cast<RectangleCollider>(collider)) {
			texturePath = "assets/images/collider_rect.png";
		}
		else if (auto circleCollider = std::dynamic_pointer_cast<CircleCollider>(collider)) {
			texturePath = "assets/images/collider_circle.png";
		}
		texture = TextureManager::loadTexture(texturePath);

		srcRect = { 0, 0, 32, 32 };
	}

	void update() override
	{
		if (tag != "terrain")
		{
			const auto x = static_cast<int>(transform->getPosition().x + colliderOffset.x) - (collider->getWidth()) / 2;
			const auto y = static_cast<int>(transform->getPosition().y + colliderOffset.y) - (collider->getHeight()) / 2;
			collider->setPosition(Vector2D(x, y));
		}

		destRect.x = collider->getPosition().x - Game::camera.x;
		destRect.y = collider->getPosition().y - Game::camera.y;
	}

	void draw() override
	{
		if (debugDraw)
			TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	std::shared_ptr<ColliderShape> getCollider() const { return collider; }
	void setColliderPos(int x, int y) { collider->setPosition(Vector2D(x, y)); }

	int getLowestPoint() const { return collider->getPosition().y + collider->getHeight(); }
	void setDebugDraw(bool value) { debugDraw = value; }

private:
	TransformComponent* transform;
	SDL_Texture* texture;

	Vector2D colliderOffset;
	SDL_Rect srcRect, destRect;

	std::shared_ptr<ColliderShape> collider;
	std::string tag;
	bool debugDraw = false;
};