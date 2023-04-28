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
#include "FactionComponent.h"

ecs::Entity* ecs::EnemySystem::instantiateEnemy(const Vector2D& pos, const std::string& filename)
{
	const auto enemyData = Game::assets->getActorJson(filename);
	auto& enemy(manager.addEntity());
	enemy.addComponent<ecs::TransformComponent>
	(
		pos.x * manager.getScale(), pos.y * manager.getScale(),
		enemyData.value("width", 0), enemyData.value("height", 0),
		manager.getScale(), enemyData["speed"]
	);
	enemy.addComponent<ecs::SpriteComponent>(enemyData["sprite_data"], true);
	enemy.addComponent<ecs::SpriteOutlineComponent>(enemyData["sprite_data"].contains("outline") ? enemyData["sprite_data"]["outline"] : nullptr);
	enemy.addComponent<ecs::ActorComponent>(filename);
	enemy.addComponent<ecs::ColliderComponent>(filename, enemyData["collider_rect"]);
	enemy.addComponent<ecs::HitboxComponent>(filename, enemyData["hitbox_rect"]);
	enemy.addComponent<ecs::HealthComponent>(enemyData["health"]);
	enemy.addComponent<ecs::ArmorComponent>();
	enemy.addComponent<ecs::AIComponentBasicEnemy>();
	enemy.addComponent<ecs::FactionComponent>(enemyData.contains("faction") ? enemyData["faction"] : "neutral");

	enemy.addGroup(Game::eGroupLabels::ENEMIES);

	return &enemy;
}

void ecs::EnemySystem::equipWeapon(ecs::Entity& enemy, const std::string& weaponName)
{
	if (!enemy.hasComponent<ecs::ActorComponent>()) return;

	const std::string enemyType = enemy.getComponent<ecs::ActorComponent>()->getActorType();

	if (enemy.hasComponent<ecs::WeaponComponent>())
	{
		if (enemy.getComponent<ecs::WeaponComponent>()->getTag() == weaponName)
			return;
		enemy.removeComponent<ecs::WeaponComponent>();
	}
	enemy.addComponent<ecs::WeaponComponent>(weaponName, enemyType);
}

void ecs::EnemySystem::equipArmor(ecs::Entity& enemy, const std::string& armorName, const std::string& slotName)
{
	if (!enemy.hasComponent<ecs::ActorComponent>()) return;

	const std::string enemyType = enemy.getComponent<ecs::ActorComponent>()->getActorType();

	if (enemy.hasComponent<ecs::ArmorComponent>())
	{
		const auto armorComponent = enemy.getComponent<ecs::ArmorComponent>();
		armorComponent->equipArmorToSlot(armorName, enemyType, slotName);
	}
}

void ecs::EnemySystem::unequipArmorPiece(ecs::Entity& enemy, const std::string& slotName)
{
	if (enemy.hasComponent<ecs::ArmorComponent>())
	{
		const auto armorComponent = enemy.getComponent<ecs::ArmorComponent>();
		armorComponent->unequipArmorFromSlot(slotName);
	}
}

void ecs::EnemySystem::unequipAllArmor(ecs::Entity& enemy)
{
	if (enemy.hasComponent<ecs::ArmorComponent>())
	{
		const auto armorComponent = enemy.getComponent<ecs::ArmorComponent>();
		armorComponent->unequipAllArmor();
	}
}


void ecs::EnemySystem::update()
{
	enemies = manager.getGroup(Game::eGroupLabels::ENEMIES);
}

void ecs::EnemySystem::draw()
{
}