#pragma once
#include "ECS.h"
#include "Observer.h"

#include "ColliderShape.h"

class Sprite;
class TransformComponent;
class SpriteComponent;
class HealthComponent;
class WeaponComponent : public DrawableComponent, private Observer
{
public:
	WeaponComponent(const std::string& name, const std::string& ownerName, bool isProjectile = false);
	~WeaponComponent() override;

	// Component
	void init() override;
	void update() override;
	void draw() override;
	// Observer
	void onNotify(const std::string_view& observedEvent) override;

	std::shared_ptr<ColliderShape> inline getCollider() const { return weaponCollider; }

	std::string inline getTag() const { return tag; }
	const std::string inline getWeaponType() const { return weaponType; }

	void inline addAffectedTarget(int id) { affectedTargets.emplace_back(id); }
	bool inline isInAffectedTargets(int id) const { return  std::find(affectedTargets.begin(), affectedTargets.end(), id) != affectedTargets.end(); }

	int inline getDamage() const { return damage; }

	bool inline isEnabled() const { return enabled; }
	bool inline  isDestroyedOnHit() const { return destroyOnHit; }
	bool inline isInRange(const std::shared_ptr<ColliderShape>& targetHitbox) const { return weaponCollider->collidesWith(targetHitbox); }

	void inline setEnableDraw(bool value) { enableDraw = value; }

private:
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::shared_ptr<ColliderShape> weaponCollider;
	SDL_Rect srcRect, destRect;
	Vector2D weaponColliderDirectionCoefficient;
	Vector2D weaponColliderOffset;
	std::string tag;
	std::string weaponType;
	SDL_Texture* texture;
	std::vector<int> affectedTargets;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Sprite>>> tempSprites;

	bool enabled;
	bool destroyOnHit;

	int damage;

	bool enableDraw{ false };
};