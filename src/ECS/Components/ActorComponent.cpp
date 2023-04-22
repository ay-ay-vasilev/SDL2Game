#include "ActorComponent.h"

#include "SpriteComponent.h"
#include "WeaponComponent.h"

#include "AssetManager.h"

ActorComponent::ActorComponent(const std::string& name)
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

void ActorComponent::init()
{
	spriteComponent = entity->getComponent<SpriteComponent>();
	registerWithSubject(spriteComponent);
	weaponComponent = entity->getComponent<WeaponComponent>();
}

void ActorComponent::playAction(const std::string& actionName)
{
	auto lockedWeapon = weaponComponent.lock();
	if (!lockedWeapon)
	{
		lockedWeapon = entity->getComponent<WeaponComponent>();
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

void ActorComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "attack_end")
	{
		notify(observedEvent);
	}
}