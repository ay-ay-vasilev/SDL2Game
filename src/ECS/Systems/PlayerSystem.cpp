#include "PlayerSystem.h"

void PlayerSystem::instantiatePlayer(const Vector2D pos, const Vector2D size, const float scale, const int speed, const std::string_view id) const
{
	auto& player(Game::manager->addEntity());
	player.addComponent<TransformComponent>(pos.x, pos.y, size.x, size.y, scale, speed);
	player.addComponent<SpriteComponent>(id, true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>(id);
	player.addGroup(Game::eGroupLabels::PLAYERS);
}

const Vector2D PlayerSystem::getPlayerPosition() const
{
	return (*players.begin())->getComponent<TransformComponent>().position;
}

void PlayerSystem::init()
{
}

void PlayerSystem::update()
{
	players = Game::manager->getGroup(Game::eGroupLabels::PLAYERS);
}

void PlayerSystem::draw()
{
	// todo remove ?
	for (const auto& c : players) c->draw();
}