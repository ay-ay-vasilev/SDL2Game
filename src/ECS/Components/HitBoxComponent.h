#pragma once
#include "Components.h"

class HitboxComponent : public Component
{
public:
	HitboxComponent(const std::string_view& tag) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitbox(),
		hitboxOffset() {};

	HitboxComponent(const std::string_view& tag, const int xpos, const int ypos, const int width, const int height) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitbox({ xpos, ypos, width, height}),
		hitboxOffset() {};

	HitboxComponent(const std::string_view& tag, const nlohmann::json& hitBoxData) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitbox({ 0, 0, hitBoxData["w"], hitBoxData["h"] }),
		hitboxOffset({ hitBoxData["dx"], hitBoxData["dy"] }) {};

	SDL_Texture* texture;
	TransformComponent* transform;
	SDL_Rect hitbox;
	SDL_Rect srcRect, destRect;
	Vector2D hitboxOffset;
	std::string tag;
	bool debugDraw = false;
	int id = 0;

	void init() override
	{
		id = entity->getID();
		if (entity->hasComponent<TransformComponent>())
		{
			transform = &entity->getComponent<TransformComponent>();
			hitboxOffset.x *= transform->scale;
			hitboxOffset.y *= transform->scale;

			hitbox.w *= static_cast<int>(transform->scale);
			hitbox.h *= static_cast<int>(transform->scale);
			hitbox.x += static_cast<int>(transform->position.x) + hitboxOffset.x - (hitbox.w) / 2;
			hitbox.y += static_cast<int>(transform->position.y) + hitboxOffset.y - (hitbox.h) / 2;
		}

		destRect = { hitbox.x, hitbox.y, hitbox.w, hitbox.h };

		texture = TextureManager::loadTexture("assets/images/hitbox.png");
		srcRect = { 0, 0, 32, 32 };
	}

	void update() override
	{
		hitbox.x = static_cast<int>(transform->position.x) + hitboxOffset.x - (hitbox.w) / 2;
		hitbox.y = static_cast<int>(transform->position.y) + hitboxOffset.y - (hitbox.h) / 2;

		destRect.x = hitbox.x - Game::camera.x;
		destRect.y = hitbox.y - Game::camera.y;
	}

	void draw() override
	{
		if (debugDraw)
			TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	void setDebugDraw(bool value) { debugDraw = value; }
};