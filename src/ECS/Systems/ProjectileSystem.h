#pragma once
#include "Components.h"

class ProjectileSystem : public System
{
public:
	ProjectileSystem(Manager& manager) : System(manager) {}

	void instantiateProjectile(const Vector2D pos, const Vector2D size, const Vector2D velocity, const int range, const int speed, const std::string_view id) const;

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> projectiles;
};