#include "EnemySystem.h"

void EnemySystem::instantiateEnemy(const Vector2D& pos, const std::string_view& filename) const
{
	std::string filenameString(filename);
	std::ifstream file(filenameString);
	if (!file.is_open())
	{
		std::cerr << "Failed to open an enemy file: " << filenameString << std::endl;
		return;
	}
	nlohmann::json enemyData;
	file >> enemyData;

	auto& enemy(manager.addEntity());
	enemy.addComponent<TransformComponent>(pos.x - (enemyData["width"] / 2), pos.y - (enemyData["height"] / 2), enemyData["width"], enemyData["height"], manager.getScale(), enemyData["speed"]);
	enemy.addComponent<SpriteComponent>(enemyData["texture"], enemyData["animations"], true);
	enemy.addComponent<ColliderComponent>(enemyData["texture"]);
	enemy.addGroup(Game::eGroupLabels::ENEMIES);
}

void EnemySystem::update()
{
	enemies = manager.getGroup(Game::eGroupLabels::ENEMIES);
}

void EnemySystem::draw()
{
}