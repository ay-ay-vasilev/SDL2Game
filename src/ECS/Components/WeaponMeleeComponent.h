#pragma once
#include "ECS.h"
#include "Observer.h"

#include "ColliderShape.h"

class Sprite;

namespace ecs
{
	class TransformComponent;
	class SpriteComponent;
	class ActorComponent;
	class HealthComponent;
	class WeaponMeleeComponent : public DrawableComponent, private Observer
	{
	public:
		WeaponMeleeComponent(const std::string& name, const std::string& ownerName, bool isProjectile = false);
		~WeaponMeleeComponent() override;

		// Component
		void init() override;
		void update(double delta) override;
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

		struct SpriteData
		{
			std::string slotName;
			std::string textureName;
			int z;
		};

		std::shared_ptr<ecs::TransformComponent> transform;
		std::shared_ptr<ecs::ActorComponent> actorComponent;
		std::shared_ptr<ecs::SpriteComponent> spriteComponent;
		std::shared_ptr<ColliderShape> weaponCollider;
		SDL_Rect srcRect, destRect;
		Vector2D weaponColliderDirectionCoefficient;
		Vector2D weaponColliderOffset;
		std::string tag;
		std::string weaponType;
		SDL_Texture* texture;
		std::vector<int> affectedTargets;
		
		std::vector<SpriteData> tempSprites;

		bool enabled;
		bool destroyOnHit;

		int damage;

		bool enableDraw{ false };
	};
}
