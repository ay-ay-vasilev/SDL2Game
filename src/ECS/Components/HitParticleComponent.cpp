#include "HitParticleComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ParticleManager.h"
#include "ParticleControllerComponent.h"

ecs::HitParticleComponent::HitParticleComponent(const std::string& name) : particleName(name) {}

void ecs::HitParticleComponent::init()
{
	setRenderOrder(1);
	transformComponent = entity->getComponent<TransformComponent>();
	healthComponent = entity->getComponent<HealthComponent>();
	registerWithSubject(healthComponent);
}

void ecs::HitParticleComponent::play()
{
	std::weak_ptr<particles::ParticleEmitter> hitParticleEmitter = Game::particleManager->addParticleEmitter(particleName);
	if (hitParticleEmitter.expired()) return;

	auto hitParticleEmitterLocked = hitParticleEmitter.lock();
	if (hitParticleEmitterLocked)
	{
		const auto entityPosition = transformComponent->getPosition();
		const auto entityDirection = transformComponent->getDirection();

		hitParticleEmitterLocked->resetSystem();
		hitParticleEmitterLocked->setAngle(hitParticleEmitterLocked->getAngle() * (entityDirection.x < 0 ? 1 : -1) + 180 * (entityDirection.x < 0 ? 0 : 1));
		hitParticleEmitterLocked->setPosition(static_cast<int>(entityPosition.x), static_cast<int>(entityPosition.y));
	}
	hitParticleEmitters.push_back(hitParticleEmitter);

	auto particleControllerComponent = entity->getComponent<ParticleControllerComponent>();
	if (particleControllerComponent) particleControllerComponent->addEmitter(hitParticleEmitter);
}

void ecs::HitParticleComponent::update(double delta)
{
	hitParticleEmitters.erase(
		std::remove_if(hitParticleEmitters.begin(), hitParticleEmitters.end(),
			[](const std::weak_ptr<particles::ParticleEmitter>& weakEmitter) {
				return weakEmitter.expired();
			}),
		hitParticleEmitters.end());

	for (auto hitParticleEmitter : hitParticleEmitters)
	{
		auto hitParticleEmitterLocked = hitParticleEmitter.lock();
		if (hitParticleEmitterLocked)
		{
			const auto position = transformComponent->getPosition();
			hitParticleEmitterLocked->setPosition(static_cast<int>(position.x), static_cast<int>(position.y));
		}
	}
}

void ecs::HitParticleComponent::draw()
{
	for (auto hitParticleEmitter : hitParticleEmitters)
	{
		auto hitParticleEmitterLocked = hitParticleEmitter.lock();
		if (hitParticleEmitterLocked)
		{
			hitParticleEmitterLocked->draw();
		}
	}
}

void ecs::HitParticleComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "hit") play();
}