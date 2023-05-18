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
#include "FactionComponent.h"
#include "CameraComponent.h"
#include "AIComponentBasicFighter.h"

#include <wrappedJson.h>

ecs::Entity* ecs::PlayerSystem::instantiatePlayer(const Vector2D& pos, const std::string& filename)
{
	const auto playerData = assets::getActorJson(filename);
	auto& player(manager.addEntity());
	player.addComponent<ecs::TransformComponent>
	(
		pos.x * manager.getScale() , pos.y * manager.getScale(),
		playerData.value("width", 0), playerData.value("height", 0),
		manager.getScale(), playerData["speed"]
	);
	player.addComponent<ecs::SpriteComponent>(playerData["sprite_data"], true);
	player.addComponent<ecs::SpriteOutlineComponent>(playerData["sprite_data"].contains("outline") ? playerData["sprite_data"]["outline"] : nullptr);
	player.addComponent<ecs::HealthComponent>(playerData["health"]);
	player.addComponent<ecs::ActorComponent>(filename);
	player.addComponent<ecs::KeyboardComponent>();
	//player.addComponent<ecs::AIComponentBasicFighter>();
	player.addComponent<ecs::ColliderComponent>(filename, playerData["collider_rect"]);
	player.addComponent<ecs::HitboxComponent>(filename, playerData["hitbox_rect"]);
	player.addComponent<ecs::ArmorComponent>();
	player.addComponent<ecs::FactionComponent>(playerData.contains("faction") ? playerData["faction"] : "neutral");
	player.addComponent<ecs::CameraComponent>();

	player.addGroup(Game::eGroupLabels::PLAYERS);
	equipWeapon(player, "unarmed");
	return &player;
}

void ecs::PlayerSystem::equipWeapon(Entity& player, const std::string& weaponName)
{
	if (!player.hasComponent<ecs::ActorComponent>()) return;

	const std::string playerType = player.getComponent<ecs::ActorComponent>()->getActorType();

	if (player.hasComponent<ecs::WeaponComponent>())
	{
		if (player.getComponent<ecs::WeaponComponent>()->getTag() == weaponName) return;
		player.removeComponent<ecs::WeaponComponent>();
	}
	player.addComponent<ecs::WeaponComponent>(weaponName, playerType);
}

const Vector2D ecs::PlayerSystem::getPlayerPosition()
{
	if (!players.empty())
	{
		lastPlayerPosition = (*players.begin())->getComponent<ecs::TransformComponent>()->getPosition();
	}
	return lastPlayerPosition;
}

void ecs::PlayerSystem::update(double delta)
{
	players = manager.getGroup(Game::eGroupLabels::PLAYERS);
}

void ecs::PlayerSystem::draw()
{
}