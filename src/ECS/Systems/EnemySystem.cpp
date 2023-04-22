#include "EnemySystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SpriteOutlineComponent.h"
#include "ColliderComponent.h"
#include "ActorComponent.h"
#include "HitboxComponent.h"
#include "HealthComponent.h"
#include "AIComponentBasicEnemy.h"
#include "ArmorComponent.h"
#include "WeaponComponent.h"

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
	enemy.addComponent<SpriteOutlineComponent>(enemyData["sprite_data"].contains("outline") ? enemyData["sprite_data"]["outline"] : nullptr);
	enemy.addComponent<ActorComponent>(filename);
	enemy.addComponent<ColliderComponent>(filename, enemyData["collider_rect"]);
	enemy.addComponent<HitboxComponent>(filename, enemyData["hitbox_rect"]);
	enemy.addComponent<HealthComponent>(enemyData["health"]);
	enemy.addComponent<ArmorComponent>();
	enemy.addComponent<AIComponentBasicEnemy>();

	enemy.addGroup(Game::eGroupLabels::ENEMIES);
	equipWeapon(enemy, "unarmed");
	equipArmor(enemy, "pants_brown", "pants");
	equipArmor(enemy, "shirt_light", "shirt");
	equipArmor(enemy, "chainmail", "jacket");
	equipArmor(enemy, "surcoat", "overcoat");
	equipArmor(enemy, "scarf_black", "mask");
	equipArmor(enemy, "eyepatch_black", "eyewear");
	equipArmor(enemy, "helmet", "headwear");

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

void EnemySystem::equipArmor(Entity& enemy, const std::string& armorName, const std::string& slotName)
{
	std::string enemyTag = "";
	if (enemy.hasComponent<HitboxComponent>())
	{
		const auto hitboxComponent = enemy.getComponent<HitboxComponent>();
		enemyTag = hitboxComponent->getTag();
	}
	if (enemy.hasComponent<ArmorComponent>())
	{
		const auto armorComponent = enemy.getComponent<ArmorComponent>();
		armorComponent->equipArmorToSlot(armorName, enemyTag, slotName);
	}
}

void EnemySystem::unequipArmorPiece(Entity& enemy, const std::string& slotName)
{
	if (enemy.hasComponent<ArmorComponent>())
	{
		const auto armorComponent = enemy.getComponent<ArmorComponent>();
		armorComponent->unequipArmorFromSlot(slotName);
	}
}

void EnemySystem::unequipAllArmor(Entity& enemy)
{
	if (enemy.hasComponent<ArmorComponent>())
	{
		const auto armorComponent = enemy.getComponent<ArmorComponent>();
		armorComponent->unequipAllArmor();
	}
}


void EnemySystem::update()
{
	enemies = manager.getGroup(Game::eGroupLabels::ENEMIES);
}

void EnemySystem::draw()
{
}