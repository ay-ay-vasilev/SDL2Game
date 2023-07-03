#include "ActorComponent.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"
#include "TintComponent.h"
#include "CorpseComponent.h"

#include "AssetManager.h"

ecs::ActorComponent::ActorComponent(const std::string& name) : actorType(name), weaponType("unarmed")
{
	nlohmann::json actorData;
	actorData = assets::getActorJson(name)["actor_data"];
}

void ecs::ActorComponent::init()
{
	spriteComponent = entity->getComponent<ecs::SpriteComponent>();
	registerWithSubject(spriteComponent);
	healthComponent = entity->getComponent<ecs::HealthComponent>();
	registerWithSubject(healthComponent);
}

void ecs::ActorComponent::update(double delta)
{
}

void ecs::ActorComponent::onNotify(const std::string_view& observedEvent)
{
	notify(observedEvent);
	if (observedEvent == entity->getID() + "_died")
	{
		std::cout << actorType << "_" << entity->getID() << " has died!\n";

		auto transformComponent = entity->getComponent<ecs::TransformComponent>();
		transformComponent->setVeloctiy(0, 0);
		playAction("death");
	}
	if (observedEvent == weaponType + "_death_end")
	{
		auto corpseComponent = entity->getComponent<ecs::CorpseComponent>();
		if (corpseComponent) corpseComponent->setCorpse(true);
		auto tintComponent = entity->getComponent<ecs::TintComponent>();
		if (tintComponent) tintComponent->enableTint("corpse");
		playAction("corpse");
	}
}

void ecs::ActorComponent::playAction(const std::string actionName)
{
	spriteComponent->play(weaponType + "_" + actionName);
}

void ecs::ActorComponent::addSprite(const std::string slotName, const std::string surfaceName, int z, const std::optional<std::string>& color)
{
	if (slotName != "weapon") actorSprites[slotName].push_back({surfaceName, z, color.has_value() ? color.value() : ""});
	spriteComponent->addSprite(slotName, std::make_shared<Sprite>(weaponType + "_" + surfaceName, z, color));
}

void ecs::ActorComponent::addBlockedSlot(const std::string blockerSlotName, const std::string blockedSlotName)
{
	spriteComponent->addBlockedSlot(blockerSlotName, blockedSlotName);
}

void ecs::ActorComponent::addBlockedSlots(const std::string& blockerName, const std::vector<std::string>& blockedSlotNames)
{
	spriteComponent->addBlockedSlots(blockerName, blockedSlotNames);
}

void ecs::ActorComponent::removeSpritesFromSlot(const std::string slotName)
{
	actorSprites[slotName].clear();
	spriteComponent->removeSpritesFromSlot(slotName);
}

void ecs::ActorComponent::setWeaponType(const std::string newWeaponType)
{
	if (weaponType != newWeaponType)
	{
		spriteComponent->removeAllSprites();
		for (const auto& [slot, spriteVec] : actorSprites)
		{
			for (const auto& sprite : spriteVec)
			{
				spriteComponent->addSprite(slot, std::make_shared<Sprite>(newWeaponType + "_" + sprite.spriteName, sprite.z, sprite.color));
			}
		}
	}
	weaponType = newWeaponType;
	playAction("idle");
}
