#pragma once

#include "Components.h"
#include "Observer.h"
#include "ColliderShape.h"

class WeaponComponent : public Component, private Observer
{
public:
	WeaponComponent(const std::string_view& tag, const nlohmann::json& weaponColliderData, bool isProjectile = false) :
		tag(tag),
		transform(nullptr), texture(nullptr), sprite(nullptr),
		srcRect(), destRect(),
		weaponColliderDirectionCoefficient({ weaponColliderData["weapon_rect"]["x"], weaponColliderData["weapon_rect"]["y"] }),
		weaponColliderOffset({ 0, 0 }),
		enabled(isProjectile),
		destroyOnHit(isProjectile),
		damage(0)
	{
		if (weaponColliderData["weapon_rect"]["shape"] == "circle")
		{
			weaponCollider = std::make_shared<CircleCollider>(Vector2D(0, 0), weaponColliderData["weapon_rect"]["radius"]);
		}
		if (weaponColliderData["weapon_rect"]["shape"] == "rectangle")
		{
			weaponCollider = std::make_shared<RectangleCollider>(Vector2D(0, 0), weaponColliderData["weapon_rect"]["w"], weaponColliderData["weapon_rect"]["h"]);
		}
		if (weaponColliderData["weapon_rect"].contains("offset")) {
			weaponColliderOffset = { weaponColliderData["weapon_rect"]["offset"]["dx"], weaponColliderData["weapon_rect"]["offset"]["dy"]};
		}
		damage = weaponColliderData.value("damage", 0);
	}

	virtual ~WeaponComponent()
	{
		sprite->removeObserver(this);
	}

	void init() override
	{
		if (entity->hasComponent<TransformComponent>())
		{
			transform = entity->getComponent<TransformComponent>();
			weaponColliderDirectionCoefficient.x *= transform->getScale();
			weaponColliderDirectionCoefficient.y *= transform->getScale();

			weaponCollider->setScale(transform->getScale());
			const auto weaponColliderX = transform->getPosition().x + weaponColliderOffset.x + transform->getDirection().x * weaponColliderDirectionCoefficient.x;
			const auto weaponColliderY = transform->getPosition().y + weaponColliderOffset.y + transform->getDirection().y * weaponColliderDirectionCoefficient.y;
			weaponCollider->setPosition(Vector2D(weaponColliderX, weaponColliderY));
		}
		if (entity->hasComponent<HitboxComponent>())
		{
			const auto hitboxComponent = entity->getComponent<HitboxComponent>();
			ownerTag = hitboxComponent->getTag();
		}
		destRect = weaponCollider->getDrawRect();

		std::string texturePath;
		if (auto rectCollider = std::dynamic_pointer_cast<RectangleCollider>(weaponCollider)) {
			texturePath = "assets/images/weapon_collider_rect.png";
		}
		else if (auto circleCollider = std::dynamic_pointer_cast<CircleCollider>(weaponCollider)) {
			texturePath = "assets/images/weapon_collider_circle.png";
		}
		texture = TextureManager::loadTexture(texturePath);
		srcRect = { 0, 0, 32, 32 };

		sprite = entity->getComponent<SpriteComponent>();
		sprite->addObserver(this);
	}

	void update() override
	{
		const auto weaponColliderX = transform->getPosition().x + weaponColliderOffset.x + transform->getDirection().x * weaponColliderDirectionCoefficient.x;
		const auto weaponColliderY = transform->getPosition().y + weaponColliderOffset.y + transform->getDirection().y * weaponColliderDirectionCoefficient.y;
		weaponCollider->setPosition(Vector2D(weaponColliderX, weaponColliderY));

		destRect = weaponCollider->getDrawRect();
		destRect.x -= Game::camera.x;
		destRect.y -= Game::camera.y;
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
		if (observedEvent == "idle_start" || observedEvent == "walk_start")
		{
			enabled = false;
		}
	}

	std::shared_ptr<ColliderShape> getCollider() const { return weaponCollider; }

	void addAffectedTarget(int id) { affectedTargets.emplace_back(id); }
	void setDebugDraw(bool value) { debugDraw = value; }

	std::string getOwnerTag() const { return ownerTag; }
	std::string getTag() const { return tag; }
	bool isInAffectedTargets(int id) const { return  std::find(affectedTargets.begin(), affectedTargets.end(), id) != affectedTargets.end(); }

	int getDamage() const { return damage; }

	bool isEnabled() const { return enabled; }
	bool isDestroyedOnHit() const { return destroyOnHit; }
	bool isInRange(const std::shared_ptr<ColliderShape>& targetHitbox) const
	{
		return weaponCollider->collidesWith(targetHitbox);
	}

private:
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<SpriteComponent> sprite;
	std::shared_ptr<ColliderShape> weaponCollider;
	SDL_Rect srcRect, destRect;
	Vector2D weaponColliderDirectionCoefficient;
	Vector2D weaponColliderOffset;
	std::string tag;
	std::string ownerTag;
	SDL_Texture* texture;
	std::vector<int> affectedTargets;

	bool enabled;
	bool destroyOnHit;
	bool debugDraw = false;

	int damage;
};