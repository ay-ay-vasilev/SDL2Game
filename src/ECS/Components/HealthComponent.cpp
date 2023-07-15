#include "HealthComponent.h"
#include "HitboxComponent.h"

ecs::HealthComponent::HealthComponent(const nlohmann::json& healthData) :
	maxHealthValue(healthData.value("max_health", 0)),
	healthValue(maxHealthValue)
{}

void ecs::HealthComponent::update(double delta)
{
	if (dead) return;

	if (healthValue <= 0)
	{
		dead = true;
		sendSignal(entity->getID() + "_died");
	}
}

void ecs::HealthComponent::sendSignal(const std::string& eventName)
{
	notify(eventName);
}

bool ecs::HealthComponent::changeHealth(int value)
{
	if (dead) return false;

	if (value < 0) sendSignal("hit");

	healthValue = std::clamp(healthValue + value, 0, maxHealthValue);
	return true;
}

bool ecs::HealthComponent::changeMaxHealth(int value)
{
	if (dead) return false;

	maxHealthValue += value;
	if (maxHealthValue <= 0)
		maxHealthValue = 0;
	return true;
}

bool ecs::HealthComponent::kill()
{
	return changeHealth(-healthValue);
}