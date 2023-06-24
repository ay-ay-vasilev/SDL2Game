#pragma once
#include "ECS.h"
#include "Observer.h"

class Sprite;

namespace ecs
{
	class TransformComponent;
	class SpriteComponent;
	class ActorComponent;
	class HealthComponent;
	class DamageColliderComponent;
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

		std::string inline getName() const { return name; }
		const std::string inline getWeaponType() const { return weaponType; }

		int inline getDamage() const { return damage; }

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
		std::shared_ptr<ecs::DamageColliderComponent> damageColliderComponent;
		std::string name;
		std::string weaponType;

		std::vector<SpriteData> tempSprites;

		int damage;
		bool isProjectile;
	};
}
