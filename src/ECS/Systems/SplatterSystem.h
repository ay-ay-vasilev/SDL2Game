#pragma once
#include "ECS.h"

class Splatter;
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
		void applySplatterToTile(Splatter& splatterData);

	private:
		std::vector<Entity*> splatterEntities;
	};
}
