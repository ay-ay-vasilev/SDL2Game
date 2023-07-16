#pragma once
#include "ECS.h"
#include "Subject.h"
#include "Observer.h"

namespace particles
{
	class ParticleEmitter;
}

namespace ecs
{
	class ParticleControllerComponent : public Component, public Observer, public Subject
	{
	public:
		explicit ParticleControllerComponent();

		// Component
		void init() override;
		void update(double delta) override;

		// Subject
		void sendSignal(const std::string& eventName);

		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		void addEmitter(const std::weak_ptr<particles::ParticleEmitter>& emitter);
		void disableEmitters();

	private:
		std::vector<std::weak_ptr<particles::ParticleEmitter>> emitters;

		bool waitForEmittersToDie{ false };
	};
}