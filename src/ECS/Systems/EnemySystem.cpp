#include "EnemySystem.h"

void EnemySystem::instantiateEnemy(const Vector2D& pos, const std::string& filename) const
{
	std::string filenameString(Game::assets->getActorJson(filename));
	std::ifstream file(filenameString);
	if (!file.is_open())
	{
		std::cerr << "Failed to open an enemy file: " << filenameString << std::endl;
		return;
	}
	nlohmann::json enemyData;
	file >> enemyData;

	auto& enemy(manager.addEntity());
	enemy.addComponent<TransformComponent>
	(
		pos.x, pos.y,
		enemyData.value("width", 0), enemyData.value("height", 0),
		manager.getScale(), enemyData["speed"]
	);
	enemy.addComponent<SpriteComponent>(enemyData["sprite_data"], true);
	enemy.addComponent<ColliderComponent>("enemy", enemyData["collider_rect"]);
	enemy.addComponent<HitboxComponent>("enemy", enemyData["hitbox_rect"]);
	enemy.addComponent<HealthComponent>(enemyData["health"]);

	enemy.addGroup(Game::eGroupLabels::ENEMIES);

}

void EnemySystem::update()
{
	enemies = manager.getGroup(Game::eGroupLabels::ENEMIES);
}

void EnemySystem::draw()
{
}