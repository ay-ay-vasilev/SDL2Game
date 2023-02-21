#include "EnemyManager.h"
#include "Components.h"

EnemyManager::EnemyManager(Manager* manager) : manager(manager) {}

EnemyManager::~EnemyManager() {}

void EnemyManager::InstantiateEnemy(const Vector2D pos, const Vector2D size, const float scale, const int speed, const std::string_view id) const
{
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, size.x, size.y, scale, speed);
	enemy.addComponent<SpriteComponent>(id, false);
	enemy.addComponent<ColliderComponent>("enemy");
	enemy.addGroup(Game::ENEMIES);
}