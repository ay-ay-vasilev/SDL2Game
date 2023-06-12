#include "SplatterComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ColliderComponent.h"

ecs::SplatterComponent::SplatterComponent(const nlohmann::json& splatterData)
{
	if (splatterData.contains("intensity")) splatterSettings.intensity = splatterData["intensity"];
	if (splatterData.contains("radius"))
	{
		splatterSettings.radiusRange.first = splatterData["radius"]["min"];
		splatterSettings.radiusRange.second = splatterData["radius"]["max"];
	}
	if (splatterData.contains("offset"))
	{
		splatterSettings.offsetRange.first = splatterData["offset"]["min"];
		splatterSettings.offsetRange.second = splatterData["offset"]["max"];
	}
	if (splatterData.contains("red"))
	{
		splatterSettings.redRange.first = splatterData["red"]["min"];
		splatterSettings.redRange.second = splatterData["red"]["max"];
	}
	if (splatterData.contains("green"))
	{
		splatterSettings.greenRange.first = splatterData["green"]["min"];
		splatterSettings.greenRange.second = splatterData["green"]["max"];
	}
	if (splatterData.contains("blue"))
	{
		splatterSettings.blueRange.first = splatterData["blue"]["min"];
		splatterSettings.blueRange.second = splatterData["blue"]["max"];
	}
	if (splatterData.contains("alpha"))
	{
		splatterSettings.alphaRange.first = splatterData["alpha"]["min"];
		splatterSettings.alphaRange.second = splatterData["alpha"]["max"];
	}
}

void ecs::SplatterComponent::init()
{
	healthComponent = entity->getComponent<HealthComponent>();
	registerWithSubject(healthComponent);
}

void ecs::SplatterComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "hit") createSplatter();
}

void ecs::SplatterComponent::createSplatter()
{
	needSplatter = true;

	const auto colliderComponent = entity->getComponent<ecs::ColliderComponent>();
	const double x = colliderComponent->getCollider()->getPosition().x;
	const double y = colliderComponent->getLowestPoint();

	splatterSettings.splatterPosition = Vector2D(x, y);
}