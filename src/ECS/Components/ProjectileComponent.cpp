#include "ProjectileComponent.h"
#include "TransformComponent.h"
#include "DamageColliderComponent.h"
#include "HealthComponent.h"
#include "SpriteComponent.h"
#include "ParticleControllerComponent.h"

ecs::ProjectileComponent::ProjectileComponent(const int ownerEntityId, Vector2D velocity, float range, bool velocityRotation) :
	ownerEntityId(ownerEntityId),
	transform(nullptr),
	velocity(velocity),
	range(range),
	velocityRotation(velocityRotation) {}

void ecs::ProjectileComponent::init()
{
	transform = entity->getComponent<ecs::TransformComponent>();
	damageCollider = entity->getComponent<ecs::DamageColliderComponent>();
	healthComponent = entity->getComponent<ecs::HealthComponent>();
	spriteComponent = entity->getComponent<ecs::SpriteComponent>();
	registerWithSubject(healthComponent);
	particleControllerComponent = entity->getComponent<ecs::ParticleControllerComponent>();
	registerWithSubject(particleControllerComponent);

	transform->setVeloctiy(velocity);
	angle = static_cast<float>(Vector2D::Angle(velocity) + 180.0);
	if (velocityRotation) transform->setRotation(angle);
	speed = transform->getSpeed();
	range *= transform->getScale();

	const float distance = damageCollider->getOffset().Distance();
	const float offsetX = distance * static_cast<float>(std::cos((angle - 180.0) * M_PI / 180.0));
	const float offsetY = distance * static_cast<float>(std::sin((angle - 180.0) * M_PI / 180.0));

	damageCollider->setOffset({offsetX, offsetY});
}

void ecs::ProjectileComponent::update(double delta)
{
	if (destroyed) return;

	if (distance > range)
	{
		std::cout << "projectile: out of range!\n";
		destroy();
	}
	distance += speed;
}

void ecs::ProjectileComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == entity->getID() + "_died")
	{
		std::cout << "projectile" << "_" << entity->getID() << " got destroyed!\n";
		destroy();
	}
	if (observedEvent == "particles_died")
	{
		entity->destroy();
	}
}

void ecs::ProjectileComponent::destroy()
{
	destroyed = true;
	transform->setVeloctiy({ 0, 0 });
	damageCollider->setEnabled(false);
	particleControllerComponent->disableEmitters();
	spriteComponent->play("destroy");
}