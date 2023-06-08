#pragma once
#include "ECS.h"
#include "Observer.h"

class ParticleEmitter;
namespace ecs
{
	class TransformComponent;
	class HealthComponent;
	class HitParticleComponent : public Component, public Observer
	{
	public:
		explicit HitParticleComponent(const std::string& name);
		virtual ~HitParticleComponent() {}

		// Component
		void init() override;
		void update(double delta) override;

		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		void play();
	private:
		std::string particleName;

		std::vector<std::weak_ptr<ParticleEmitter>> hitParticleEmitters;
		std::shared_ptr<TransformComponent> transformComponent;
		std::shared_ptr<HealthComponent> healthComponent;
	};
}