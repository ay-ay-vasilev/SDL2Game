#include "DebugParticleComponent.h"
#include "TransformComponent.h"
#include "ParticleManager.h"

ecs::DebugParticleComponent::DebugParticleComponent(const std::string& name) : particleName(name) {}

void ecs::DebugParticleComponent::init()
{
	setRenderOrder(2);
	transformComponent = entity->getComponent<TransformComponent>();
}

void ecs::DebugParticleComponent::play()
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
		emitterLocked->setStartSize(emitterLocked->getStartSize() * Game::constants->SCALE);
		emitterLocked->setEndSize(emitterLocked->getEndSize() * Game::constants->SCALE);
		emitterLocked->setPosition(static_cast<int>(entityPosition.x), static_cast<int>(entityPosition.y));
	}
}

void ecs::DebugParticleComponent::update(double delta)
{
	//if (emitter.expired())
	//{
	//	if (active) active = false;
	//	return;
	//}

	//auto emitterLocked = emitter.lock();
	//if (emitterLocked)
	//{
	//	const auto position = transformComponent->getPosition();
	//	emitterLocked->setPosition(position.x, position.y);
	//}
}

void ecs::DebugParticleComponent::draw()
{
	auto emitterLocked = emitter.lock();
	if (emitterLocked)
	{
		emitterLocked->draw();
	}
}