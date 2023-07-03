#pragma once
#include "ECS.h"

#include "Events/EventManager.h"
#include "Events/Events.h"

class Splatter;
namespace ecs
{
	class SplatterSystem : public System
	{
	public:
		explicit SplatterSystem(Manager& manager) : System(manager) {}
		~SplatterSystem() override;
		// System
		void init() override;
		void update(double delta) override;
		void draw() override;

		void createSplatter(SplatterEvent* splatterData);
		void applySplatterToTile(const Splatter& splatterData);

	private:
		std::vector<Entity*> splatterEntities;
		ListenerHandle listenerHandle;
	};
}
