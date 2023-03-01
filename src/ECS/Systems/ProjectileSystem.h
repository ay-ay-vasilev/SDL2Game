#pragma once
#include "Components.h"

class ProjectileSystem : public System
{
public:
	ProjectileSystem(Manager& manager) : System(manager) {}

	void instantiateProjectile(const Vector2D pos, const Vector2D velocity, const std::string_view& filename) const;

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> projectiles;
};