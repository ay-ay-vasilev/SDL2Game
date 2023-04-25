#include "PlayerSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SpriteOutlineComponent.h"
#include "ColliderComponent.h"
#include "ActorComponent.h"
#include "KeyboardComponent.h"
#include "HitboxComponent.h"
#include "HealthComponent.h"
#include "ArmorComponent.h"
#include "WeaponComponent.h"

#include <wrappedJson.h>

Entity* PlayerSystem::instantiatePlayer(const Vector2D& pos, const std::string& filename)
{
	const auto playerData = Game::assets->getActorJson(filename);
	auto& player(manager.addEntity());
	player.addComponent<TransformComponent>
	(
		pos.x * manager.getScale() , pos.y * manager.getScale(),
		playerData.value("width", 0), playerData.value("height", 0),
		manager.getScale(), playerData["speed"]
	);
	player.addComponent<SpriteComponent>(playerData["sprite_data"], true);
	player.addComponent<SpriteOutlineComponent>(playerData["sprite_data"].contains("outline") ? playerData["sprite_data"]["outline"] : nullptr);
	player.addComponent<ActorComponent>(filename);
	player.addComponent<KeyboardComponent>();
	player.addComponent<ColliderComponent>(filename, playerData["collider_rect"]);
	player.addComponent<HitboxComponent>(filename, playerData["hitbox_rect"]);
	player.addComponent<HealthComponent>(playerData["health"]);
	player.addComponent<ArmorComponent>();

	player.addGroup(Game::eGroupLabels::PLAYERS);
	equipWeapon(player, "unarmed");
	return &player;
}

void PlayerSystem::equipWeapon(Entity& player, const std::string& weaponName)
{
	if (!player.hasComponent<ActorComponent>()) return;

	const std::string playerType = player.getComponent<ActorComponent>()->getActorType();

	if (player.hasComponent<WeaponComponent>())
	{
		if (player.getComponent<WeaponComponent>()->getTag() == weaponName) return;
		player.removeComponent<WeaponComponent>();
	}
	player.addComponent<WeaponComponent>(weaponName, playerType);
}

void PlayerSystem::equipArmor(Entity& player, const std::string& armorName, const std::string& slotName)
{
	if (!player.hasComponent<ActorComponent>()) return;

	const std::string playerType = player.getComponent<ActorComponent>()->getActorType();

	if (player.hasComponent<ArmorComponent>())
	{
		const auto armorComponent = player.getComponent<ArmorComponent>();
		armorComponent->equipArmorToSlot(armorName, playerType, slotName);
	}
}

void PlayerSystem::unequipArmor(Entity& player, const std::string& slotName)
{
	if (player.hasComponent<ArmorComponent>())
	{
		const auto armorComponent = player.getComponent<ArmorComponent>();
		armorComponent->unequipArmorFromSlot(slotName);
	}
}

const Vector2D PlayerSystem::getPlayerPosition()
{
	if (!players.empty())
	{
		lastPlayerPosition = (*players.begin())->getComponent<TransformComponent>()->getPosition();
	}
	return lastPlayerPosition;
}

void PlayerSystem::update()
{
	players = manager.getGroup(Game::eGroupLabels::PLAYERS);
}

void PlayerSystem::draw()
{
}