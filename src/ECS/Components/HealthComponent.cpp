#include "HealthComponent.h"
#include "HitboxComponent.h"

HealthComponent::HealthComponent(const nlohmann::json& healthData)
{
	maxHealthValue = healthData.value("max_health", 0);
	healthValue = maxHealthValue;
}

void HealthComponent::update()
{
	if (healthValue <= 0)
	{
		// move tag to entity instead?
		sendSignal(entity->getComponent<HitboxComponent>()->getTag() + "_died");
		entity->destroy();
	}
}

void HealthComponent::sendSignal(const std::string& eventName)
{
	std::cout << eventName << "\n";
	notify(eventName);
}

bool HealthComponent::changeHealth(int value)
{
	healthValue = std::clamp(healthValue + value, 0, maxHealthValue);
	return true;
}

bool HealthComponent::changeMaxHealth(int value)
{
	maxHealthValue += value;
	if (maxHealthValue <= 0)
		maxHealthValue = 0;
	return true;
}
