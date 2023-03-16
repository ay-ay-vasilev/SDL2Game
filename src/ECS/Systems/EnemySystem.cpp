#include "EnemySystem.h"

Entity* EnemySystem::instantiateEnemy(const Vector2D& pos, const std::string& filename) const
{
	const auto enemyData = Game::assets->getActorJson(filename);
	auto& enemy(manager.addEntity());
	enemy.addComponent<TransformComponent>
	(
		pos.x * manager.getScale(), pos.y * manager.getScale(),
		enemyData.value("width", 0), enemyData.value("height", 0),
		manager.getScale(), enemyData["speed"]
	);
	enemy.addComponent<SpriteComponent>(enemyData["sprite_data"], true);
	enemy.addComponent<ColliderComponent>("enemy", enemyData["collider_rect"]);
	enemy.addComponent<HitboxComponent>("enemy", enemyData["hitbox_rect"]);
	enemy.addComponent<HealthComponent>(enemyData["health"]);
	enemy.addComponent<AIComponentBasicEnemy>();
	enemy.addGroup(Game::eGroupLabels::ENEMIES);

	return &enemy;
}

void EnemySystem::equipWeapon(Entity& enemy, const std::string& weaponName)
{
	enemy.addComponent<WeaponComponent>(weaponName);
}

void EnemySystem::update()
{
	enemies = manager.getGroup(Game::eGroupLabels::ENEMIES);
}

void EnemySystem::draw()
{
}