#pragma once

#include "Components.h"
#include "Observer.h"

class WeaponComponent : public Component, private Observer
{
public:
	WeaponComponent(const std::string_view& tag, const nlohmann::json& weaponColliderData, bool isProjectile = false) :
		tag(tag),
		transform(nullptr), texture(nullptr), sprite(nullptr),
		srcRect(), destRect(),
		weaponCollider({ 0, 0, weaponColliderData["w"], weaponColliderData["h"] }),
		weaponColliderDirectionCoefficient({ weaponColliderData["x"], weaponColliderData["y"] }),
		weaponColliderOffset({ 0, 0 }),
		enabled(isProjectile)
	{
		if (weaponColliderData.contains("offset")) {
			weaponColliderOffset = { weaponColliderData["offset"]["dx"], weaponColliderData["offset"]["dy"] };
		}
	}

	void init() override
	{
		if (entity->hasComponent<TransformComponent>())
		{
			transform = &entity->getComponent<TransformComponent>();
			weaponColliderDirectionCoefficient.x *= transform->getScale();
			weaponColliderDirectionCoefficient.y *= transform->getScale();

			weaponCollider.w *= static_cast<int>(transform->getScale());
			weaponCollider.h *= static_cast<int>(transform->getScale());
			weaponCollider.x = static_cast<int>(transform->getPosition().x + weaponColliderOffset.x + transform->getDirection().x * weaponColliderDirectionCoefficient.x) - (weaponCollider.w) / 2;
			weaponCollider.y = static_cast<int>(transform->getPosition().y + weaponColliderOffset.y + transform->getDirection().y * weaponColliderDirectionCoefficient.y) - (weaponCollider.h) / 2;
		}
		if (entity->hasComponent<HitboxComponent>())
		{
			const auto hitboxComponent = &entity->getComponent<HitboxComponent>();
			ownerTag = hitboxComponent->getTag();
		}

		destRect = { weaponCollider.x, weaponCollider.y, weaponCollider.w, weaponCollider.h };

		texture = TextureManager::loadTexture("assets/images/weapon_collider.png");
		srcRect = { 0, 0, 32, 32 };

		sprite = &entity->getComponent<SpriteComponent>();
		sprite->addObserver(this);
	}

	void update() override
	{
		if (!enabled)
			return;

		weaponCollider.x = static_cast<int>(transform->getPosition().x + weaponColliderOffset.x + transform->getDirection().x * weaponColliderDirectionCoefficient.x) - (weaponCollider.w) / 2;
		weaponCollider.y = static_cast<int>(transform->getPosition().y + weaponColliderOffset.y + transform->getDirection().y * weaponColliderDirectionCoefficient.y) - (weaponCollider.h) / 2;

		destRect.x = weaponCollider.x - Game::camera.x;
		destRect.y = weaponCollider.y - Game::camera.y;
	}

	void draw() override
	{
		if (!enabled)
			return;

		if (debugDraw)
			TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	void onNotify(const std::string_view& observedEvent) override
	{
		if (observedEvent == "attack_action_start")
		{
			enabled = true;
		}
		if (observedEvent == "attack_action_stop")
		{
			enabled = false;
			affectedTargets.clear();
		}
	}

	SDL_Rect getCollider() const { return weaponCollider; }

	void addAffectedTarget(int id) { affectedTargets.emplace_back(id); }
	void setDebugDraw(bool value) { debugDraw = value; }

	bool isEnabled() const { return enabled; }
	std::string getOwnerTag() const { return ownerTag; }
	std::string getTag() const { return tag; }
	bool isInAffectedTargets(int id) const { return  std::find(affectedTargets.begin(), affectedTargets.end(), id) != affectedTargets.end(); }

private:
	TransformComponent* transform;
	SpriteComponent* sprite;
	SDL_Rect weaponCollider;
	SDL_Rect srcRect, destRect;
	Vector2D weaponColliderDirectionCoefficient;
	Vector2D weaponColliderOffset;
	std::string tag;
	std::string ownerTag;
	SDL_Texture* texture;
	std::vector<int> affectedTargets;

	bool enabled;
	bool debugDraw = false;

};