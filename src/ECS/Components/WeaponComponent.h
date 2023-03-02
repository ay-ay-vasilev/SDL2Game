#pragma once

#include "Components.h"
#include "Observer.h"

class WeaponComponent : public Component, private Observer
{
public:
	WeaponComponent(const std::string_view& tag, const nlohmann::json& weaponColliderData) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		weaponCollider({ 0, 0, weaponColliderData["w"], weaponColliderData["h"] }),
		weaponColliderOffset({ weaponColliderData["dx"], weaponColliderData["dy"] }) {};

	SDL_Rect weaponCollider;
	Vector2D weaponColliderOffset;
	std::string tag;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	TransformComponent* transform;
	SpriteComponent* sprite;

	bool isEnabled = false;
	bool debugDraw = false;

	void init() override
	{
		if (entity->hasComponent<TransformComponent>())
		{
			transform = &entity->getComponent<TransformComponent>();
			weaponColliderOffset.x *= transform->scale;
			weaponColliderOffset.y *= transform->scale;

			weaponCollider.w *= static_cast<int>(transform->scale);
			weaponCollider.h *= static_cast<int>(transform->scale);
			weaponCollider.x += static_cast<int>(transform->position.x) + weaponColliderOffset.x - (weaponCollider.w) / 2;
			weaponCollider.y += static_cast<int>(transform->position.y) + weaponColliderOffset.y - (weaponCollider.h) / 2;
		}

		destRect = { weaponCollider.x, weaponCollider.y, weaponCollider.w, weaponCollider.h };

		texture = TextureManager::loadTexture("assets/images/weapon_collider.png");
		srcRect = { 0, 0, 32, 32 };

		sprite = &entity->getComponent<SpriteComponent>();
		sprite->addObserver(this);
	}

	void update() override
	{
		if (!isEnabled)
			return;

		weaponCollider.x = static_cast<int>(transform->position.x) + (transform->direction.x * weaponColliderOffset.x) - (weaponCollider.w) / 2;
		weaponCollider.y = static_cast<int>(transform->position.y) + (transform->direction.y * weaponColliderOffset.y) - (weaponCollider.h) / 2;

		destRect.x = weaponCollider.x - Game::camera.x;
		destRect.y = weaponCollider.y - Game::camera.y;
	}

	void draw() override
	{
		if (!isEnabled)
			return;

		if (debugDraw)
			TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	void onNotify(const std::string_view& observedEvent) override
	{
		if (observedEvent == "attack_action_start")
		{
			isEnabled = true;
		}
		if (observedEvent == "attack_action_stop")
		{
			isEnabled = false;
		}
	}

	void setDebugDraw(bool value) { debugDraw = value; }
};