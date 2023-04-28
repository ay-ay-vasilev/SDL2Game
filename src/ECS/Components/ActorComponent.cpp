#include "ActorComponent.h"

#include "SpriteComponent.h"
#include "WeaponComponent.h"

#include "AssetManager.h"

ecs::ActorComponent::ActorComponent(const std::string& name) : actorType(name)
{
	nlohmann::json actorData;
	actorData = Game::assets->getActorJson(name)["actor_data"];

	if (actorData.contains("weapon_type_animations"))
	{
		for (auto& [weaponType, animMappingData] : actorData["weapon_type_animations"].items())
			for (auto& [actionName, animName] : animMappingData.items())
				actionAnimationsByWeaponType[weaponType][actionName] = animName;
	}
}

void ecs::ActorComponent::init()
{
	spriteComponent = entity->getComponent<ecs::SpriteComponent>();
	registerWithSubject(spriteComponent);
	weaponComponent = entity->getComponent<ecs::WeaponComponent>();
}

void ecs::ActorComponent::playAction(const std::string& actionName)
{
	auto lockedWeapon = weaponComponent.lock();
	if (!lockedWeapon)
	{
		lockedWeapon = entity->getComponent<ecs::WeaponComponent>();
		weaponComponent = lockedWeapon;
	}
	if (lockedWeapon)
	{
		const auto& weaponType = lockedWeapon->getWeaponType();
		if (actionAnimationsByWeaponType.count(weaponType) != 0 &&
			actionAnimationsByWeaponType[weaponType].count(actionName) != 0)
		{
			spriteComponent->play(actionAnimationsByWeaponType[weaponType][actionName]);
			return;
		}
	}
	spriteComponent->play(actionName);
}

void ecs::ActorComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "attack_end")
	{
		notify(observedEvent);
	}
}