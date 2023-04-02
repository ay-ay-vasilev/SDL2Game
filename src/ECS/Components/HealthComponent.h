#pragma once
#include "ECS.h"
#include "HitboxComponent.h"
#include "Subject.h"

class HealthComponent : public Component, public Subject
{
public:
	HealthComponent(const nlohmann::json& healthData)
	{
		maxHealthValue = healthData.value("max_health", 0);
		healthValue = maxHealthValue;
	}

	~HealthComponent()
	{
	}

	void update() override
	{
		if (healthValue <= 0)
		{
			// move tag to entity instead?
			sendSignal(entity->getComponent<HitboxComponent>()->getTag() + "_died");
			entity->destroy();
		}
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

	void sendSignal(const std::string& eventName)
	{
		std::cout << eventName << "\n";
		notify(eventName);
	}

private:
	int healthValue;
	int maxHealthValue;
};