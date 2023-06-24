#include "WeaponMeleeComponent.h"
#include "TransformComponent.h"
#include "ActorComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"
#include "DamageColliderComponent.h"
#include "AssetManager.h"

ecs::WeaponMeleeComponent::WeaponMeleeComponent(const std::string& name, const std::string& ownerName, bool isProjectile) :
	name(name),
	damage(0),
	isProjectile(isProjectile)
{
	nlohmann::json weaponData;
	if (isProjectile) weaponData = assets::getProjectileJson(name)["weapon"];
	else weaponData = assets::getWeaponJson(name);
	damage = weaponData.value("damage", 0);
	weaponType = weaponData.value("weapon_type", "unarmed");

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
}

ecs::WeaponMeleeComponent::~WeaponMeleeComponent()
{
}

void ecs::WeaponMeleeComponent::init()
{
	actorComponent = entity->getComponent<ecs::ActorComponent>();
	if (actorComponent)
	{
		registerWithSubject(actorComponent);

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
	else
	{
		spriteComponent = entity->getComponent<ecs::SpriteComponent>();
		registerWithSubject(spriteComponent);

		if (tempSprites.empty()) spriteComponent->removeSpritesFromSlot("weapon");
		for (const auto& spriteData : tempSprites)
			spriteComponent->removeSpritesFromSlot(spriteData.slotName);
		for (const auto& spriteData : tempSprites)
		{
			spriteComponent->addSprite(spriteData.slotName, std::make_shared<Sprite>(spriteData.textureName, spriteData.z));
		}
		tempSprites.clear();
	}
	damageColliderComponent = entity->getComponent<ecs::DamageColliderComponent>();
	damageColliderComponent->loadParams(name, isProjectile);
}

void ecs::WeaponMeleeComponent::update(double delta)
{
}

void ecs::WeaponMeleeComponent::draw()
{
}

void ecs::WeaponMeleeComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "attack_action_start")
	{
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