#pragma once
#include "ECS.h"

namespace particles
{
	class ParticleEmitter;
}

namespace ecs
{
	class TransformComponent;
	class ParticleComponent : public DrawableComponent
	{
	public:
		explicit ParticleComponent(const std::string& name);

		// Component
		void init() override;
		void update(double delta) override;
		void draw() override;

		void play();
	private:
		bool active = false;

		std::string particleName;

		std::weak_ptr<particles::ParticleEmitter> emitter;
		std::shared_ptr<TransformComponent> transformComponent;
	};
}