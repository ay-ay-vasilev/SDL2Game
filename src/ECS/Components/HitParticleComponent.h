#pragma once
#include "ECS.h"

class ParticleEmitter;
namespace ecs
{
	class TransformComponent;
	class HitParticleComponent : public Component
	{
	public:
		explicit HitParticleComponent(const std::string& name);
		virtual ~HitParticleComponent() {}

		// Component
		void init() override;
		void update(double delta) override;

		void play();
	private:
		std::string particleName;

		std::weak_ptr<ParticleEmitter> particleEmitter;
		std::shared_ptr<TransformComponent> transformComponent;
	};
}