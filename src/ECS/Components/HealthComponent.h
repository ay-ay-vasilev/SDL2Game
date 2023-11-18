#pragma once
#include "ECS.h"
#include "Subject.h"

#include <wrappedJson.h>

namespace ecs
{
	class HitboxComponent;
	class HealthComponent : public Component, public Subject
	{
	public:
		explicit HealthComponent(const nlohmann::json& healthData);

		// Component
		void update(double delta) override;

		// Subject
		void sendSignal(const std::string& eventName);

		bool changeHealth(int value);
		bool changeMaxHealth(int value);
		bool kill();

		const int inline getHealth() const { return healthValue; }
		const int inline getMaxHealth() const { return maxHealthValue; }

		const bool inline isDead() const { return dead; }

	private:
		int maxHealthValue;
		int healthValue;
		bool dead{false};
	};
}