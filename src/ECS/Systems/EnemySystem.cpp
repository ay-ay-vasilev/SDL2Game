#include "EnemySystem.h"

void EnemySystem::instantiateEnemy(const Vector2D pos, const Vector2D size, const float scale, const int speed, const std::string_view id) const
{
	auto& enemy(manager.addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, size.x, size.y, scale, speed);
	enemy.addComponent<SpriteComponent>(id, true);
	enemy.addComponent<ColliderComponent>("enemy");
	enemy.addGroup(Game::eGroupLabels::ENEMIES);
}

void EnemySystem::update()
{
	enemies = manager.getGroup(Game::eGroupLabels::ENEMIES);
}

void EnemySystem::draw()
{
}