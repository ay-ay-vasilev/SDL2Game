#pragma once
#include "Components.h"

class HealthComponent : public Component
{
public:
	HealthComponent(const nlohmann::json& healthData)
	{
		maxHealthValue = healthData.value("max_health", 0);
		healthValue = maxHealthValue;
	}

	void update() override
	{
	}

	bool changeHealth(int value)
	{
		healthValue = std::clamp(healthValue + value, 0, maxHealthValue);
		return true;
	}

	bool changeMaxHealth(int value)
	{
		maxHealthValue += value;
		if (maxHealthValue <= 0)
			maxHealthValue = 0;
		return true;
	}

	int getHealth() const { return healthValue; }
	int getMaxHealth() const { return maxHealthValue; }

private:
	int healthValue;
	int maxHealthValue;
};