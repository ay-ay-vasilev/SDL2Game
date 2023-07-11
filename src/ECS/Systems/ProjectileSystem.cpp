#include "ProjectileSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SpriteOutlineComponent.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "HitboxComponent.h"
#include "DamageColliderComponent.h"
#include "HealthComponent.h"
#include "ParticleComponent.h"

#include <wrappedJson.h>

ecs::ProjectileSystem::~ProjectileSystem()
{
	events::EventManager::remove_listener<events::ProjectileEvent>(*listenerHandle.get());
}

void ecs::ProjectileSystem::init()
{
	listenerHandle = std::make_unique<ProjectileEventListenerHandle>(events::EventManager::listen<events::ProjectileEvent>(this, &ecs::ProjectileSystem::createProjectile));
}

void ecs::ProjectileSystem::createProjectile(const events::ProjectileEvent* projectileData)
{
	instantiateProjectile(projectileData->ownerId, projectileData->position, projectileData->velocity, projectileData->filename);
}

void ecs::ProjectileSystem::instantiateProjectile(const int ownerId, const Vector2D pos, const Vector2D velocity, const std::string& filename) const
{
	std::string filenameString(assets::getProjectileJsonPath(filename));
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
		pos.x, pos.y,
		projectileData.value("width", 0), projectileData.value("height", 0),
		manager.getScale(), projectileData["speed"]
	);
	projectile.addComponent<ecs::SpriteComponent>(projectileData["sprite_data"], false);
	projectile.addComponent<ecs::SpriteOutlineComponent>(projectileData["sprite_data"].contains("outline") ? projectileData["sprite_data"]["outline"] : nullptr);
	projectile.addComponent<ecs::HitboxComponent>(filename, projectileData["hitbox_rect"]);
	projectile.addComponent<ecs::DamageColliderComponent>(filename, true);
	projectile.addComponent<ecs::ColliderComponent>(filename, projectileData["collider_rect"]);
	projectile.addComponent<ecs::HealthComponent>(projectileData["projectile_data"]["health"]);
	projectile.addComponent<ecs::ParticleComponent>(projectileData["particle"]);
	projectile.addComponent<ecs::ProjectileComponent>
		(
			ownerId,
			velocity,
			projectileData["projectile_data"]["range"],
			projectileData["projectile_data"]["velocity_rotation"]
		);

	projectile.addGroup(Game::PROJECTILES);
}

void ecs::ProjectileSystem::update(double delta)
{
	projectiles = manager.getGroup(Game::eGroupLabels::PROJECTILES);
}

void ecs::ProjectileSystem::draw()
{
}