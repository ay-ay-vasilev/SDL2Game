#include "WeaponComponent.h"
#include "TransformComponent.h"
#include "ActorComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"
#include "DamageColliderComponent.h"
#include "AssetManager.h"

ecs::WeaponComponent::WeaponComponent(const std::string& name) :
	name(name)
{
}

ecs::WeaponComponent::~WeaponComponent()
{
}

void ecs::WeaponComponent::init()
{
	actorComponent = entity->getComponent<ecs::ActorComponent>();
	registerWithSubject(actorComponent);
	damageColliderComponent = entity->getComponent<ecs::DamageColliderComponent>();

	loadWeaponData();
}

void ecs::WeaponComponent::update(double delta)
{
}

void ecs::WeaponComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "attack_action_start")
	{
		// add ranged weapon logic here
		damageColliderComponent->setEnabled(true);
	}
	if (observedEvent == "attack_action_stop")
	{
		damageColliderComponent->setEnabled(false);
		damageColliderComponent->clearAffectedTargets();
	}
	if (observedEvent == "idle_start" || observedEvent == "walk_start")
	{
		damageColliderComponent->setEnabled(false);
	}
}

void ecs::WeaponComponent::loadWeaponData()
{
	damageColliderComponent->assignDamageCollider(name);

	const nlohmann::json weaponData = assets::getWeaponJson(name);
	weaponType = weaponData.value("weapon_type", "unarmed");
	const auto ownerName = actorComponent->getActorType();
	
	if (weaponData.contains("sprite_data"))
	{
		if (weaponData["sprite_data"].contains(ownerName))
		{
			const auto ownerWeaponData = weaponData["sprite_data"][ownerName];
			const auto weaponSpriteData = ownerWeaponData["sprites"];

			for (const auto data : weaponSpriteData)
			{
				const auto tempZ = data.value("z", 0);
				tempSprites.push_back({ data["slot"], data["texture"], tempZ });
			}
		}
	}
	actorComponent->setWeaponType(weaponType);
	if (tempSprites.empty()) actorComponent->removeSpritesFromSlot("weapon");
	for (const auto& spriteData : tempSprites)
		actorComponent->removeSpritesFromSlot(spriteData.slotName);
	for (const auto& spriteData : tempSprites)
	{
		actorComponent->addSprite(spriteData.slotName, spriteData.textureName, spriteData.z);
	}
	tempSprites.clear();
}

void ecs::WeaponComponent::equipWeapon(const std::string& name)
{
	this->name = name;
	loadWeaponData();
}