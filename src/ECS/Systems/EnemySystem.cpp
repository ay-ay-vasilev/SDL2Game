#include "EnemySystem.h"

Entity* EnemySystem::instantiateEnemy(const Vector2D& pos, const std::string& filename)
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
	enemy.addComponent<ActorComponent>(filename);
	enemy.addComponent<ColliderComponent>(filename, enemyData["collider_rect"]);
	enemy.addComponent<HitboxComponent>(filename, enemyData["hitbox_rect"]);
	enemy.addComponent<HealthComponent>(enemyData["health"]);
	enemy.addComponent<AIComponentBasicEnemy>();

	enemy.addGroup(Game::eGroupLabels::ENEMIES);
	equipWeapon(enemy, "unarmed");

	return &enemy;
}

void EnemySystem::equipWeapon(Entity& enemy, const std::string& weaponName)
{
	std::string enemyTag = "";
	if (enemy.hasComponent<HitboxComponent>())
	{
		const auto hitboxComponent = enemy.getComponent<HitboxComponent>();
		enemyTag = hitboxComponent->getTag();
	}
	if (enemy.hasComponent<WeaponComponent>())
	{
		if (enemy.getComponent<WeaponComponent>()->getTag() == weaponName)
			return;
		enemy.removeComponent<WeaponComponent>();
	}
	enemy.addComponent<WeaponComponent>(weaponName, enemyTag);
}

void EnemySystem::update()
{
	enemies = manager.getGroup(Game::eGroupLabels::ENEMIES);
}

void EnemySystem::draw()
{
}