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

		std::string inline getTag() const { return tag; }
		const std::string inline getWeaponType() const { return weaponType; }

		int inline getDamage() const { return damage; }

		bool inline isEnabled() const { return enabled; }
		bool inline  isDestroyedOnHit() const { return destroyOnHit; }

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
		std::string tag;
		std::string weaponType;
		SDL_Texture* texture;
		
		std::vector<SpriteData> tempSprites;

		bool enabled;
		bool destroyOnHit;

		int damage;

		bool enableDraw{ false };
	};
}
