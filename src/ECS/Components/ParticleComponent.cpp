#include "ParticleComponent.h"
#include "TransformComponent.h"
#include "ParticleManager.h"
#include "ParticleControllerComponent.h"

ecs::ParticleComponent::ParticleComponent(const std::string& name) : particleName(name) {}

void ecs::ParticleComponent::init()
{
	setRenderOrder(-10);
	transformComponent = entity->getComponent<TransformComponent>();
	play();
}

void ecs::ParticleComponent::play()
{
	emitter = Game::particleManager->addParticleEmitter(particleName);
	if (emitter.expired()) return;

	auto emitterLocked = emitter.lock();
	if (emitterLocked)
	{
		const auto entityPosition = transformComponent->getPosition();
		const auto entityDirection = transformComponent->getDirection();

		emitterLocked->resetSystem();
		emitterLocked->setAngle(emitterLocked->getAngle() * (entityDirection.x < 0 ? 1 : -1) + 180 * (entityDirection.x < 0 ? 0 : 1));
		emitterLocked->setPosition(static_cast<int>(entityPosition.x), static_cast<int>(entityPosition.y));
	}

	auto particleControllerComponent = entity->getComponent<ParticleControllerComponent>();
	if (particleControllerComponent)
		particleControllerComponent->addEmitter(emitter);
}

void ecs::ParticleComponent::update(double delta)
{
	if (emitter.expired())
	{
		if (active) active = false;
		return;
	}

	auto emitterLocked = emitter.lock();
	if (emitterLocked)
	{
		const auto position = transformComponent->getPosition();
		emitterLocked->setPosition(static_cast<int>(position.x), static_cast<int>(position.y));
	}
}

void ecs::ParticleComponent::draw()
{
	auto emitterLocked = emitter.lock();
	if (emitterLocked)
	{
		emitterLocked->draw();
	}
}