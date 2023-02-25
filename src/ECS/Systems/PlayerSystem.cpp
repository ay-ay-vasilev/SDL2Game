#include "PlayerSystem.h"

#include <json.hpp>

void PlayerSystem::instantiatePlayer(const Vector2D& pos, const std::string_view& filename) const
{
	std::string filenameString(filename);
	std::ifstream file(filenameString);
	if (!file.is_open())
	{
		std::cerr << "Failed to open a player file: " << filenameString << std::endl;
		return;
	}
	nlohmann::json playerData;
	file >> playerData;

	auto& player(manager.addEntity());
	player.addComponent<TransformComponent>(pos.x - (playerData["width"] / 2), pos.y - (playerData["height"] / 2), playerData["width"], playerData["height"], manager.getScale(), playerData["speed"]);
	player.addComponent<SpriteComponent>(playerData["texture"], playerData["animations"], true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>(playerData["texture"]);
	player.addGroup(Game::eGroupLabels::PLAYERS);
}

const Vector2D PlayerSystem::getPlayerPosition() const
{
	return (*players.begin())->getComponent<TransformComponent>().position;
}

void PlayerSystem::update()
{
	players = manager.getGroup(Game::eGroupLabels::PLAYERS);
}

void PlayerSystem::draw()
{
}