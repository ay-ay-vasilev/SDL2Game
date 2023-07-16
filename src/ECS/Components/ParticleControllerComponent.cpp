#include "ParticleControllerComponent.h"
#include "Particles/ParticleEmitter.h"
#include "HealthComponent.h"

ecs::ParticleControllerComponent::ParticleControllerComponent() {}

void ecs::ParticleControllerComponent::init()
{
	auto healthComponent = entity->getComponent<ecs::HealthComponent>();
	if (healthComponent)
	{
		registerWithSubject(healthComponent);
	}
}

void ecs::ParticleControllerComponent::update(double delta)
{
	emitters.erase(
		std::remove_if(emitters.begin(), emitters.end(),
			[](const std::weak_ptr<particles::ParticleEmitter>& weakEmitter) {
				return weakEmitter.expired();
			}),
		emitters.end());

	if (waitForEmittersToDie)
	{
		if (emitters.empty())
		{
			waitForEmittersToDie = false;
			sendSignal("particles_died");
		}
	}
}

void ecs::ParticleControllerComponent::sendSignal(const std::string& eventName)
{
	notify(eventName);
}

void ecs::ParticleControllerComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == entity->getID() + "_died")
	{
		waitForEmittersToDie = true;
	}
}

void ecs::ParticleControllerComponent::addEmitter(const std::weak_ptr<particles::ParticleEmitter>& emitter)
{
	emitters.emplace_back(emitter);
}

void ecs::ParticleControllerComponent::disableEmitters()
{
	for (const auto& emitter : emitters)
	{
		auto emitterLocked = emitter.lock();
		if (emitterLocked)
		{
			emitterLocked->stopSystem();
		}
	}
}