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
	if (!particleEmitter.expired())
	{
		if (particleEmitter.lock()->isActive()) return;
		else
		{
			Game::particleManager->removeParticleEmitter(particleEmitter.lock());
			particleEmitter.reset();
		}
	}

	particleEmitter = Game::particleManager->addParticleEmitter(particleName);
	auto particleEmitterLocked = particleEmitter.lock();
	if (particleEmitterLocked)
	{
		const auto entityPosition = transformComponent->getPosition();
		const auto entityDirection = transformComponent->getDirection();

		particleEmitterLocked->resetSystem();
		particleEmitterLocked->setAngle(particleEmitterLocked->getAngle() * (entityDirection.x < 0 ? 1 : -1) + 180 * (entityDirection.x < 0 ? 0 : 1));
		particleEmitterLocked->setStartSize(particleEmitterLocked->getStartSize() * Game::constants->SCALE);
		particleEmitterLocked->setEndSize(particleEmitterLocked->getEndSize() * Game::constants->SCALE);
		particleEmitterLocked->setPosition(entityPosition.x, entityPosition.y);
	}
}

void ecs::HitParticleComponent::update(double delta)
{
	auto particleEmitterLocked = particleEmitter.lock();
	if (particleEmitterLocked)
	{
		const auto position = transformComponent->getPosition();
		particleEmitterLocked->setPosition(position.x, position.y);
	}
}