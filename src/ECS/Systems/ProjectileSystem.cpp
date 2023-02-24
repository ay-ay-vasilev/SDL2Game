#include "ProjectileSystem.h"

void ProjectileSystem::instantiateProjectile(const Vector2D pos, const Vector2D size, const Vector2D velocity, const int range, const int speed, const std::string_view id) const
{
	auto& projectile(Game::manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, size.x, size.y, 1);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(velocity, range, speed);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::PROJECTILES);
}

void ProjectileSystem::init()
{
}

void ProjectileSystem::update()
{
	projectiles = Game::manager->getGroup(Game::eGroupLabels::PROJECTILES);
}

void ProjectileSystem::draw()
{
	// todo remove ?
	for (const auto& c : projectiles) c->draw();
}