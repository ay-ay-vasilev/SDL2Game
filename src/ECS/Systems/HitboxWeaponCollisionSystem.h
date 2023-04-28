#pragma once
#include "ECS.h"

namespace ecs
{
	class HitboxWeaponCollisionSystem : public System
	{
	public:
		explicit HitboxWeaponCollisionSystem(Manager& manager) : System(manager) {}

		void update() override;
		void draw() override;

	private:

		std::vector<Entity*> hitboxes;
		std::vector<Entity*> weapons;
	};
}
