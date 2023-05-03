#pragma once
#include "ECS.h"
#include "Observer.h"

namespace ecs
{
	class ActorComponent;
	class TransformComponent;
	class HitboxComponent;
	class ArmorComponent;
	class KeyboardComponent : public Component, private Observer
	{
	public:
		virtual ~KeyboardComponent() {}

		enum class eState
		{
			IDLE,
			WALK,
			ATTACK
		};

		// Component
		void init() override;
		// Observer
		void onNotify(const std::string_view& observedEvent);

		void handleEvents();

	private:
		void equipWeapon(const std::string& weaponName);
		void equipArmor();
		void unequipArmor();

		bool pressed = false;
		eState state = eState::IDLE;
		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<ActorComponent> actorComponent;
	};
}