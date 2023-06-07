#pragma once
#include "ECS.h"

class ParticleEmitter;
namespace ecs
{
	class TransformComponent;
	class DebugParticleComponent : public Component
	{
	public:
		explicit DebugParticleComponent(const std::string& name);
		virtual ~DebugParticleComponent() {}

		// Component
		void init() override;
		void update(double delta) override;

		void play();
	private:
		bool active = false;

		std::string particleName;

		std::weak_ptr<ParticleEmitter> emitter;
		std::shared_ptr<TransformComponent> transformComponent;
	};
}