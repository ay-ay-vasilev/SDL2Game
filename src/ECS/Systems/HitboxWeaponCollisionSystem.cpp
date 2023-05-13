#include "HitboxWeaponCollisionSystem.h"
#include "HitboxComponent.h"
#include "WeaponComponent.h"
#include "HealthComponent.h"
#include "ArmorComponent.h"
#include "ActorComponent.h"
#include "FactionComponent.h"
#include "Collision.h"
#include "Vector2D.h"
#include "Game.h"

void ecs::HitboxWeaponCollisionSystem::update(double delta)
{
	hitboxes = manager.getEntitiesWithComponent<ecs::HitboxComponent>();
	weapons = manager.getEntitiesWithComponent<ecs::WeaponComponent>();

	for (const auto hitbox : hitboxes)
	{
		const auto hitboxComponent = hitbox->getComponent<ecs::HitboxComponent>();
		hitboxComponent->setEnableDraw(Game::constants->DRAW_HITBOXES);
	}

	for (const auto weapon : weapons)
	{
		const auto weaponComponent = weapon->getComponent<ecs::WeaponComponent>();
		weaponComponent->setEnableDraw(Game::constants->DRAW_HITBOXES);
	}

	for (auto hitboxEntity : hitboxes)
	{
		auto hitboxCollider = hitboxEntity->getComponent<ecs::HitboxComponent>();
		auto hitboxFaction = hitboxEntity->getComponent<ecs::FactionComponent>();

		for (auto weaponWieldingEntity : weapons)
		{
			// check if hits itself
			if (hitboxEntity->getID() == weaponWieldingEntity->getID())
				continue;

			// check if hits friend
			auto weaponWielderFaction = weaponWieldingEntity->getComponent<ecs::FactionComponent>();
			if (hitboxFaction && weaponWielderFaction && hitboxFaction->checkIfFactionFriendly(weaponWielderFaction->getFaction()))
				continue;

			auto weaponCollider = weaponWieldingEntity->getComponent<ecs::WeaponComponent>();

			if (!weaponCollider->isEnabled())
				continue;
			if (weaponCollider->isInAffectedTargets(hitboxEntity->getID()))
				continue;

			if (weaponCollider->getCollider()->collidesWith(hitboxCollider->getHitbox()))
			{
				weaponCollider->addAffectedTarget(hitboxEntity->getID());
				auto actorHealthComponent = hitboxEntity->getComponent<ecs::HealthComponent>();
				auto actorArmorComponent = hitboxEntity->getComponent<ecs::ArmorComponent>();

				const auto weaponDamage = weaponCollider->getDamage();
				const auto damage = actorArmorComponent ? actorArmorComponent->applyDamageReduction(weaponDamage) : weaponDamage;
				actorHealthComponent->changeHealth(-damage);
				const int currentHealth = actorHealthComponent->getHealth();

				// =========== LOGGING ===========
				auto entityName = weaponCollider->getTag() + "_" + std::to_string(hitboxEntity->getID());
				if (hitboxEntity->hasComponent<ecs::ActorComponent>())
				{
					const auto actorComponent = hitboxEntity->getComponent<ecs::ActorComponent>();
					entityName = actorComponent->getActorType() + "_" + std::to_string(hitboxEntity->getID());
				}
				std::cout
					<< entityName << " got hit by " << weaponCollider->getTag() << " for " << damage << " damage! "
					<< entityName << " has " << currentHealth << " hp left!\n";
				// ===============================

				if (weaponCollider->isDestroyedOnHit())
				{
					weaponWieldingEntity->destroy();
				}
			}
		}
	}
}

void ecs::HitboxWeaponCollisionSystem::draw()
{
}