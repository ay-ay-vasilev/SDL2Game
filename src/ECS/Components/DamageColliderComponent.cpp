#include "DamageColliderComponent.h"
#include "TransformComponent.h"



void ecs::DamageColliderComponent::update(double delta)
{
	const auto weaponColliderX = transform->getPosition().x + weaponColliderOffset.x + transform->getDirection().x * weaponColliderDirectionCoefficient.x;
	const auto weaponColliderY = transform->getPosition().y + weaponColliderOffset.y + transform->getDirection().y * weaponColliderDirectionCoefficient.y;
	damageCollider->setPosition(Vector2D(weaponColliderX, weaponColliderY));

	destRect = damageCollider->getDrawRect();
}