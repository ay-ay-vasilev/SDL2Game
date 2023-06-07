#include "HitParticleComponent.h"
#include "TransformComponent.h"
#include "ParticleManager.h"

ecs::HitParticleComponent::HitParticleComponent(const std::string& name) : particleName(name) {}

void ecs::HitParticleComponent::init()
{
	transformComponent = entity->getComponent<TransformComponent>();
}

void ecs::HitParticleComponent::play()
{
	std::weak_ptr<ParticleEmitter> hitParticleEmitter = Game::particleManager->addParticleEmitter(particleName);
	if (hitParticleEmitter.expired()) return;

	auto hitParticleEmitterLocked = hitParticleEmitter.lock();
	if (hitParticleEmitterLocked)
	{
		const auto entityPosition = transformComponent->getPosition();
		const auto entityDirection = transformComponent->getDirection();

		hitParticleEmitterLocked->resetSystem();
		hitParticleEmitterLocked->setAngle(hitParticleEmitterLocked->getAngle() * (entityDirection.x < 0 ? 1 : -1) + 180 * (entityDirection.x < 0 ? 0 : 1));
		hitParticleEmitterLocked->setStartSize(hitParticleEmitterLocked->getStartSize() * Game::constants->SCALE);
		hitParticleEmitterLocked->setEndSize(hitParticleEmitterLocked->getEndSize() * Game::constants->SCALE);
		hitParticleEmitterLocked->setPosition(entityPosition.x, entityPosition.y);
	}
	hitParticleEmitters.push_back(hitParticleEmitter);
}

void ecs::HitParticleComponent::update(double delta)
{
	hitParticleEmitters.erase(
		std::remove_if(hitParticleEmitters.begin(), hitParticleEmitters.end(),
			[](const std::weak_ptr<ParticleEmitter>& weakEmitter) {
				return weakEmitter.expired();
			}),
		hitParticleEmitters.end());

	for (auto hitParticleEmitter : hitParticleEmitters)
	{
		auto hitParticleEmitterLocked = hitParticleEmitter.lock();
		if (hitParticleEmitterLocked)
		{
			const auto position = transformComponent->getPosition();
			hitParticleEmitterLocked->setPosition(position.x, position.y);
		}
	}
}