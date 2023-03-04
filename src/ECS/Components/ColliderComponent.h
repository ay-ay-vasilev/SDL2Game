#pragma once
#include "Components.h"
#include "TextureManager.h"

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

	ColliderComponent(const std::string_view& tag, const int xpos, const int ypos, const int size) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider({ xpos, ypos, size, size }),
		colliderOffset() {};

	ColliderComponent(const std::string_view& tag, const int xpos, const int ypos, const int width, const int height) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider({ xpos, ypos, width, height }),
		colliderOffset() {};

	ColliderComponent(const std::string_view& tag, const nlohmann::json& colliderData) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider({ 0, 0, colliderData["w"], colliderData["h"]}),
		colliderOffset ({ colliderData["dx"], colliderData["dy"] }) {};
	
	void init() override
	{
		if (entity->hasComponent<TransformComponent>())
		{
			transform = &entity->getComponent<TransformComponent>();
			colliderOffset.x *= transform->getScale();
			colliderOffset.y *= transform->getScale();

			collider.w *= static_cast<int>(transform->getScale());
			collider.h *= static_cast<int>(transform->getScale());
			collider.x += static_cast<int>(transform->getPosition().x) + colliderOffset.x - (collider.w) / 2;
			collider.y += static_cast<int>(transform->getPosition().y) + colliderOffset.y - (collider.h) / 2;
		}

		destRect = { collider.x, collider.y, collider.w, collider.h };

		texture = TextureManager::loadTexture("assets/images/collider.png");
		srcRect = { 0, 0, 32, 32 };
	}

	void update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->getPosition().x) + colliderOffset.x - (collider.w) / 2;
			collider.y = static_cast<int>(transform->getPosition().y) + colliderOffset.y - (collider.h) / 2;
		}

		destRect.x = collider.x - Game::camera.x;
		destRect.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		if (debugDraw)
			TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	SDL_Rect getCollider() const { return collider; }
	void setColliderPos(int x, int y) { collider.x = x; collider.y = y; }

	int getLowestPoint() const { return collider.y + collider.h; }
	void setDebugDraw(bool value) { debugDraw = value; }

private:
	TransformComponent* transform;
	SDL_Texture* texture;

	Vector2D colliderOffset;
	SDL_Rect srcRect, destRect;

	SDL_Rect collider;
	std::string tag;
	bool debugDraw = false;
};