#pragma once
#include "Components.h"

class HitBoxComponent : public Component
{
public:
	HitBoxComponent(const std::string_view tag) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitBox(),
		hitBoxOffset() {};

	HitBoxComponent(const std::string_view tag, const int xpos, const int ypos, const int width, const int height) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitBox({ xpos, ypos, width, height}),
		hitBoxOffset() {};

	HitBoxComponent(const std::string_view tag, const nlohmann::json& hitBoxData) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitBox({ 0, 0, hitBoxData["w"], hitBoxData["h"] }),
		hitBoxOffset({ hitBoxData["dx"], hitBoxData["dy"] }) {};

	SDL_Rect hitBox;
	Vector2D hitBoxOffset;
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
			hitBoxOffset.x *= transform->scale;
			hitBoxOffset.y *= transform->scale;

			hitBox.w *= static_cast<int>(transform->scale);
			hitBox.h *= static_cast<int>(transform->scale);
			hitBox.x += static_cast<int>(transform->position.x) + hitBoxOffset.x - (hitBox.w) / 2;
			hitBox.y += static_cast<int>(transform->position.y) + hitBoxOffset.y - (hitBox.h) / 2;
		}

		destRect = { hitBox.x, hitBox.y, hitBox.w, hitBox.h };

		texture = TextureManager::loadTexture("assets/images/hitbox.png");
		srcRect = { 0, 0, 32, 32 };
	}

	void update() override
	{
		if (tag != "terrain")
		{
			hitBox.x = static_cast<int>(transform->position.x) + hitBoxOffset.x - (hitBox.w) / 2;
			hitBox.y = static_cast<int>(transform->position.y) + hitBoxOffset.y - (hitBox.h) / 2;
		}

		destRect.x = hitBox.x - Game::camera.x;
		destRect.y = hitBox.y - Game::camera.y;
	}

	void draw() override
	{
		if (debugDraw)
			TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	void setDebugDraw(bool value) { debugDraw = value; }
};