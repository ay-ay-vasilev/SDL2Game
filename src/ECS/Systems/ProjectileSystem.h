#pragma once
#include "ECS.h"

namespace ecs
{
	class ProjectileSystem : public System
	{
	public:
		explicit ProjectileSystem(Manager& manager) : System(manager) {}

		void instantiateProjectile(const Vector2D pos, const Vector2D velocity, const std::string& filename) const;

		void update(double delta) override;
		void draw() override;

	private:
		std::vector<Entity*> projectiles;
	};
}