#pragma once
#include "ECS.h"

#include "Events/EventManager.h"
#include "Events/Events.h"

namespace ecs
{
	using ProjectileEventListenerHandle = events::CallbackContainer<events::ProjectileEvent>::ListenerHandle;

	class ProjectileSystem : public System
	{
	public:
		explicit ProjectileSystem(Manager& manager) : System(manager) {}
		~ProjectileSystem() override;

		void instantiateProjectile(const Uint32 ownerId, const Vector2D pos, const Vector2D velocity, const std::string& filename) const;
		void createProjectile(const events::ProjectileEvent* projectileData);

		// System
		void init() override;
		void update(double delta) override;
		void draw() override;

	private:
		std::unique_ptr<ProjectileEventListenerHandle> listenerHandle;
	};
}