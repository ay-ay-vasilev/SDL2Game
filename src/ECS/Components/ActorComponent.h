#pragma once
#include "ECS.h"
#include "AssetManager.h"
#include "SpriteComponent.h"
#include "WeaponComponent.h"

class ActorComponent : public Component
{
public:
	ActorComponent(const std::string_view& name)
	{
		const auto actorName = std::string(name);
		nlohmann::json actorData;
		actorData = Game::assets->getActorJson(actorName)["actor_data"];

		if (actorData.contains("weapon_type_animations"))
		{
			for (auto& [weaponType, animMappingData] : actorData["weapon_type_animations"].items())
			{
				for (auto& [actionName, animName] : animMappingData.items())
				{
					actionAnimationsByWeaponType[weaponType][actionName] = animName;
				}
			}
		}
	}
	virtual ~ActorComponent() {}
	
	void init() override
	{
		spriteComponent = entity->getComponent<SpriteComponent>();
		weaponComponent = entity->getComponent<WeaponComponent>();
	}

	void playAction(const std::string& actionName)
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

private:
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::weak_ptr<WeaponComponent> weaponComponent;

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> actionAnimationsByWeaponType;
};