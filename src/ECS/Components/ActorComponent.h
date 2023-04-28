#pragma once
#include "ECS.h"
#include "Observer.h"
#include "Subject.h"

namespace ecs
{
	class SpriteComponent;
	class WeaponComponent;
	class ActorComponent : public Component, public Observer, public Subject
	{
	public:
		explicit ActorComponent(const std::string& name);
		virtual ~ActorComponent() {}

		// Component
		void init() override;

		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		void playAction(const std::string& actionName);

		const std::string inline getActorType() const { return actorType; }

	private:
		std::shared_ptr<ecs::SpriteComponent> spriteComponent;
		std::weak_ptr<ecs::WeaponComponent> weaponComponent;
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> actionAnimationsByWeaponType;

		std::string actorType;
	};
}