#include "HitboxWeaponCollisionSystem.h"
#include "HitboxComponent.h"
#include "DamageColliderComponent.h"
#include "HealthComponent.h"
#include "ArmorComponent.h"
#include "ActorComponent.h"
#include "FactionComponent.h"
#include "ProjectileComponent.h"
#include "Collision.h"
#include "Vector2D.h"
#include "Game.h"

void ecs::HitboxWeaponCollisionSystem::update(double delta)
{
	hitboxes = manager.getEntitiesWithComponent<ecs::HitboxComponent>();
	damageColliders = manager.getEntitiesWithComponent<ecs::DamageColliderComponent>();

	for (const auto hitbox : hitboxes)
	{
		const auto hitboxComponent = hitbox->getComponent<ecs::HitboxComponent>();
		hitboxComponent->setEnableDraw(Game::constants->DRAW_HITBOXES);
	}

	for (const auto damageCollider : damageColliders)
	{
		const auto damageColliderComponent = damageCollider->getComponent<ecs::DamageColliderComponent>();
		damageColliderComponent->setEnableDraw(Game::constants->DRAW_HITBOXES);
	}

	for (auto hitboxEntity : hitboxes)
	{
		auto actorHealthComponent = hitboxEntity->getComponent<ecs::HealthComponent>();
		if (actorHealthComponent && actorHealthComponent->isDead()) continue;

		auto hitboxCollider = hitboxEntity->getComponent<ecs::HitboxComponent>();
		auto hitboxFaction = hitboxEntity->getComponent<ecs::FactionComponent>();

		for (auto damageColliderEntity : damageColliders)
		{
			const auto damageColliderHealth = damageColliderEntity->getComponent<ecs::HealthComponent>();
			if (damageColliderHealth && damageColliderHealth->isDead()) continue;

			// check if hits itself
			if (hitboxEntity->getID() == damageColliderEntity->getID()) continue;

			// check if hits friend
			const auto damageColliderFaction = damageColliderEntity->getComponent<ecs::FactionComponent>();
			if (hitboxFaction && damageColliderFaction && hitboxFaction->checkIfFactionFriendly(damageColliderFaction->getFaction()))
				continue;

			// check if it's a projectile
			const auto projectileComponent = damageColliderEntity->getComponent<ecs::ProjectileComponent>();
			if (projectileComponent)
			{
				// check if hits its owner
				if (projectileComponent->getOwnerEntityId() == hitboxEntity->getID())
					continue;

				// check if hitbox entity is a projectile and has the same owner
				const auto hitboxProjectileComponent = hitboxEntity->getComponent<ecs::ProjectileComponent>();
				if (hitboxProjectileComponent && projectileComponent->getOwnerEntityId() == hitboxProjectileComponent->getOwnerEntityId())
					continue;
			}

			auto damageColliderComponent = damageColliderEntity->getComponent<ecs::DamageColliderComponent>();

			if (!damageColliderComponent->isEnabled())
				continue;
			if (damageColliderComponent->isInAffectedTargets(hitboxEntity->getID()))
				continue;

			if (damageColliderComponent->getCollider()->collidesWith(hitboxCollider->getHitbox()))
			{
				if (!damageColliderComponent->addAffectedTarget(hitboxEntity->getID()))
					return;

				auto actorArmorComponent = hitboxEntity->getComponent<ecs::ArmorComponent>();

				const auto weaponDamage = damageColliderComponent->getDamage();
				const auto damage = actorArmorComponent ? actorArmorComponent->applyDamageReduction(weaponDamage) : weaponDamage;
				actorHealthComponent->changeHealth(-damage);
				const int currentHealth = actorHealthComponent->getHealth();

				// =========== LOGGING ===========
				//std::string entityName = "";
				//if (hitboxEntity->hasComponent<ecs::ActorComponent>())
				//{
				//	const auto actorComponent = hitboxEntity->getComponent<ecs::ActorComponent>();
				//	entityName = actorComponent->getActorType() + "_" + std::to_string(hitboxEntity->getID());
				//}
				//else if (hitboxEntity->hasComponent<ecs::DamageColliderComponent>()) // todo : maybe add a name to the projectile component ?
				//{
				//	const auto hitboxDamageColliderComponent = hitboxEntity->getComponent<ecs::DamageColliderComponent>();
				//	entityName = hitboxDamageColliderComponent->getTag() + "_" + std::to_string(hitboxEntity->getID());
				//}
				//std::cout
				//	<< entityName << " got hit by " << damageColliderComponent->getTag() << " for " << damage << " damage! "
				//	<< entityName << " has " << currentHealth << " hp left!\n";
				// ===============================

				if (damageColliderComponent->isDestroyedOnHit())
				{
					if (damageColliderHealth) damageColliderHealth->kill();
				}
			}
		}
	}
}

void ecs::HitboxWeaponCollisionSystem::draw()
{
}