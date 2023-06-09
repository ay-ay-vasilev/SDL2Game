#pragma once
#include "ECS.h"

namespace ecs
{
	class SplatterSystem : public System
	{
	public:
		explicit SplatterSystem(Manager& manager) : System(manager) {}
		// System
		void init() override;
		void update(double delta) override;
		void draw() override;

		void checkForSplatter();
		void applySplatterToTile(Vector2D pos);

	private:
		std::vector<Entity*> splatterEntities;
	};
}
