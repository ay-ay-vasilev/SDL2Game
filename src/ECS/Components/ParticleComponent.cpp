#include "ParticleComponent.h"
#include "TransformComponent.h"
#include "ParticleManager.h"

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
		emitterLocked->setPosition(position.x, position.y);
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