#include "HealthComponent.h"
#include "HitboxComponent.h"

ecs::HealthComponent::HealthComponent(const nlohmann::json& healthData) :
	maxHealthValue(healthData.value("max_health", 0)),
	healthValue(maxHealthValue)
{}

void ecs::HealthComponent::update()
{
	if (healthValue <= 0)
	{
		sendSignal(entity->getID() + "_died");
		entity->destroy();
	}
}

void ecs::HealthComponent::sendSignal(const std::string& eventName)
{
	std::cout << eventName << "\n";
	notify(eventName);
}

bool ecs::HealthComponent::changeHealth(int value)
{
	healthValue = std::clamp(healthValue + value, 0, maxHealthValue);
	return true;
}

bool ecs::HealthComponent::changeMaxHealth(int value)
{
	maxHealthValue += value;
	if (maxHealthValue <= 0)
		maxHealthValue = 0;
	return true;
}
