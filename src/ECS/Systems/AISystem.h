#pragma once
#include "ECS.h"

namespace ecs
{
	class AISystem : public System
	{
	public:
		explicit AISystem(Manager& manager) : System(manager) {}

		void update() override;
		void draw() override;

	private:
		std::vector<Entity*> aiActors;
		std::vector<Entity*> entitiesWithFactions;
	};
}
