#pragma once
#include "ECS.h"
#include "Subject.h"

class HitboxComponent;
class HealthComponent : public Component, public Subject
{
public:
	explicit HealthComponent(const nlohmann::json& healthData);

	// Component
	void update() override;

	// Subject
	void sendSignal(const std::string& eventName);

	bool changeHealth(int value);
	bool changeMaxHealth(int value);

	const int inline getHealth() const { return healthValue; }
	const int inline getMaxHealth() const { return maxHealthValue; }

private:
	int maxHealthValue;
	int healthValue;
};