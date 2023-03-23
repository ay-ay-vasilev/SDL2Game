#include "PlayerSystem.h"

#include <json.hpp>

Entity* PlayerSystem::instantiatePlayer(const Vector2D& pos, const std::string& filename) const
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
	player.addComponent<KeyboardComponent>();
	player.addComponent<ColliderComponent>(filename, playerData["collider_rect"]);
	player.addComponent<HitboxComponent>(filename, playerData["hitbox_rect"]);
	player.addComponent<HealthComponent>(playerData["health"]);

	player.addGroup(Game::eGroupLabels::PLAYERS);

	return &player;
}

void PlayerSystem::equipWeapon(Entity& player, const std::string& weaponName)
{
	std::string playerTag = "";
	if (player.hasComponent<HitboxComponent>())
	{
		const auto hitboxComponent = player.getComponent<HitboxComponent>();
		playerTag = hitboxComponent->getTag();
	}
	player.addComponent<WeaponComponent>(weaponName, playerTag);
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