#pragma once
#include "Components.h"
#include "TextureManager.h"

#include <SDL.h>
#include <json.hpp>

#include <string>


class ColliderComponent : public Component
{
public:
	ColliderComponent(const std::string_view tag) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider(),
		colliderOffset() {};

	ColliderComponent(const std::string_view tag, const int xpos, const int ypos, const int size) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider({ xpos, ypos, size, size }),
		colliderOffset() {};

	ColliderComponent(const std::string_view tag, const int xpos, const int ypos, const int width, const int height) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider({ xpos, ypos, width, height }),
		colliderOffset() {};

	ColliderComponent(const std::string_view tag, const nlohmann::json& colliderData) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		collider({ 0, 0, colliderData["w"], colliderData["h"]}),
		colliderOffset ({ colliderData["dx"], colliderData["dy"] }) {};
	
	SDL_Rect collider;
	Vector2D colliderOffset;
	std::string tag;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	TransformComponent* transform;
	bool debugDraw = false;

	void init() override
	{
		if (entity->hasComponent<TransformComponent>())
		{
			transform = &entity->getComponent<TransformComponent>();
			colliderOffset.x *= transform->scale;
			colliderOffset.y *= transform->scale;

			collider.w *= static_cast<int>(transform->scale);
			collider.h *= static_cast<int>(transform->scale);
			collider.x += static_cast<int>(transform->position.x) + colliderOffset.x - (collider.w) / 2;
			collider.y += static_cast<int>(transform->position.y) + colliderOffset.y - (collider.h) / 2;
		}

		destRect = { collider.x, collider.y, collider.w, collider.h };

		texture = TextureManager::loadTexture("assets/images/collider.png");
		srcRect = { 0, 0, 32, 32 };
	}

	void update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x) + colliderOffset.x - (collider.w) / 2;
			collider.y = static_cast<int>(transform->position.y) + colliderOffset.y - (collider.h) / 2;
		}

		destRect.x = collider.x - Game::camera.x;
		destRect.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		if (debugDraw)
			TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	int getLowestPoint() { return collider.y + collider.h; }
	void setDebugDraw(bool value) { debugDraw = value; }
};