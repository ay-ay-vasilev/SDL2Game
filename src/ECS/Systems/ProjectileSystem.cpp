#include "ProjectileSystem.h"

#include <json.hpp>

void ProjectileSystem::instantiateProjectile(const Vector2D pos, const Vector2D velocity, const std::string& filename) const
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
	projectile.addComponent<TransformComponent>
	(
		pos.x * manager.getScale(), pos.y * manager.getScale(),
		projectileData.value("width", 0), projectileData.value("height", 0),
		manager.getScale(), projectileData["speed"]
	);
	projectile.addComponent<SpriteComponent>(projectileData["sprite_data"], false);
	projectile.addComponent<ProjectileComponent>(velocity, projectileData["projectile_data"]["range"]);
	projectile.addComponent<HitboxComponent>("projectile", projectileData["hitbox_rect"]);
	projectile.addComponent<WeaponComponent>("test_projectile", nullptr, true);
	projectile.addComponent<ColliderComponent>("projectile", projectileData["collider_rect"]);
	projectile.addComponent<HealthComponent>(projectileData["projectile_data"]["health"]);

	projectile.addGroup(Game::PROJECTILES);
}

void ProjectileSystem::update()
{
	projectiles = manager.getGroup(Game::eGroupLabels::PROJECTILES);
}

void ProjectileSystem::draw()
{
}