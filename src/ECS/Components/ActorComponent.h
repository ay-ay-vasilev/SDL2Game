#pragma once
#include "ECS.h"
#include "Observer.h"
#include "Subject.h"

namespace ecs
{
	class SpriteComponent;
	class WeaponComponent;
	class HealthComponent;
	class ActorComponent : public Component, public Observer, public Subject
	{
	public:
		explicit ActorComponent(const std::string& name);
		virtual ~ActorComponent() {}

		// Component
		void init() override;
		void update(double delta) override;

		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		// SpriteComponent interface
		void addSprite(const std::string slotName, const std::string surfaceName, int z, const std::optional<std::string>& color = std::nullopt);
		void addBlockedSlot(const std::string blockerSlotName, const std::string blockedSlotName);
		void addBlockedSlots(const std::string& blockerName, const std::vector<std::string>& blockedSlotNames);
		void removeSpritesFromSlot(const std::string slotName);

		void playAction(const std::string actionName);

		const std::string inline getActorType() const { return actorType; }
		const std::string inline getWeaponType() const { return weaponType; }

		void setWeaponType(const std::string newWeaponType);

	private:
		struct SpriteData
		{
			std::string spriteName;
			int z;
			std::string color = "";
		};

		std::shared_ptr<ecs::SpriteComponent> spriteComponent;
		std::shared_ptr<ecs::HealthComponent> healthComponent;
		std::weak_ptr<ecs::WeaponComponent> weaponComponent;

		std::unordered_map<std::string, std::vector<SpriteData>> actorSprites;

		std::string actorType;
		std::string weaponType;
	};
}