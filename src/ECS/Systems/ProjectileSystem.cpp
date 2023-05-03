#include "ProjectileSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "HitboxComponent.h"
#include "WeaponComponent.h"
#include "HealthComponent.h"

#include <wrappedJson.h>

void ecs::ProjectileSystem::instantiateProjectile(const Vector2D pos, const Vector2D velocity, const std::string& filename) const
{
	std::string filenameString(Game::assets->getProjectileJsonPath(filename));
	std::ifstream file(filenameString);
	if (!file.is_open())
	{
		std::cerr << "Failed to open a projectile file: " << filenameString << std::endl;
		return;
	}
	nlohmann::json projectileData;
	file >> projectileData;

	auto& projectile(manager.addEntity());
	projectile.addComponent<ecs::TransformComponent>
	(
		pos.x * manager.getScale(), pos.y * manager.getScale(),
		projectileData.value("width", 0), projectileData.value("height", 0),
		manager.getScale(), projectileData["speed"]
	);
	projectile.addComponent<ecs::SpriteComponent>(projectileData["sprite_data"], false);
	projectile.addComponent<ecs::ProjectileComponent>(velocity, projectileData["projectile_data"]["range"]);
	projectile.addComponent<ecs::HitboxComponent>(filename, projectileData["hitbox_rect"]);
	projectile.addComponent<ecs::WeaponComponent>(filename, "", true);
	projectile.addComponent<ecs::ColliderComponent>(filename, projectileData["collider_rect"]);
	projectile.addComponent<ecs::HealthComponent>(projectileData["projectile_data"]["health"]);

	projectile.addGroup(Game::PROJECTILES);
}

void ecs::ProjectileSystem::update(double delta)
{
	projectiles = manager.getGroup(Game::eGroupLabels::PROJECTILES);
}

void ecs::ProjectileSystem::draw()
{
}