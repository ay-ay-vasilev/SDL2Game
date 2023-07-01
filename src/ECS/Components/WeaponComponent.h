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
	class WeaponComponent : public Component, private Observer
	{
	public:
		WeaponComponent(const std::string& name = "unarmed");
		~WeaponComponent() override;

		// Component
		void init() override;
		void update(double delta) override;
		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		void loadWeaponData();
		void equipWeapon(const std::string& name);

		std::string inline getName() const { return name; }
		const std::string inline getWeaponType() const { return weaponType; }

	private:


		struct SpriteData
		{
			std::string slotName;
			std::string textureName;
			int z;
		};

		std::shared_ptr<ecs::TransformComponent> transform;
		std::shared_ptr<ecs::ActorComponent> actorComponent;
		std::shared_ptr<ecs::DamageColliderComponent> damageColliderComponent;
		std::string name;
		std::string weaponType;

		std::vector<SpriteData> tempSprites;
	};
}
