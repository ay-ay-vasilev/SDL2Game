#pragma once
#include "ECS.h"

#include "Events/EventManager.h"
#include "Events/Events.h"

class Splatter;

namespace ecs
{
	using SplatterEventListenerHandle = events::CallbackContainer<events::SplatterEvent>::ListenerHandle;

	class SplatterSystem : public System
	{
	public:
		explicit SplatterSystem(Manager& manager) : System(manager) {}
		~SplatterSystem() override;
		// System
		void init() override;

		void createSplatter(const events::SplatterEvent* splatterData);
		void applySplatterToTile(const Splatter& splatterData);

	private:
		std::vector<Entity*> splatterEntities;
		std::unique_ptr<SplatterEventListenerHandle> listenerHandle;
	};
}
