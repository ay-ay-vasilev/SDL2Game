#include "PlayerSystem.h"

#include <json.hpp>

void PlayerSystem::instantiatePlayer(const Vector2D& pos, const std::string& filename) const
{
	std::string filenameString(Game::assets->getActorJson(filename));
	std::ifstream file(filenameString);
	if (!file.is_open())
	{
		std::cerr << "Failed to open a player file: " << filenameString << std::endl;
		return;
	}
	nlohmann::json playerData;
	file >> playerData;

	auto& player(manager.addEntity());
	player.addComponent<TransformComponent>
	(
		pos.x * manager.getScale() , pos.y * manager.getScale(),
		playerData.value("width", 0), playerData.value("height", 0),
		manager.getScale(), playerData["speed"]
	);
	player.addComponent<SpriteComponent>(playerData["sprite_data"], true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player", playerData["collider_rect"]);
	player.addComponent<HitboxComponent>("player", playerData["hitbox_rect"]);
	player.addComponent<WeaponComponent>("sword", playerData["weapon"]);
	player.addComponent<HealthComponent>(playerData["health"]);

	player.addGroup(Game::eGroupLabels::PLAYERS);
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