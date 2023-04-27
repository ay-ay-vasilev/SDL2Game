#include "HitboxWeaponCollisionSystem.h"
#include "HitboxComponent.h"
#include "WeaponComponent.h"
#include "HealthComponent.h"
#include "ArmorComponent.h"
#include "ActorComponent.h"
#include "FactionComponent.h"
#include "Collision.h"
#include "Vector2D.h"

void HitboxWeaponCollisionSystem::update()
{
	hitboxes = manager.getEntitiesWithComponent<HitboxComponent>();
	weapons = manager.getEntitiesWithComponent<WeaponComponent>();

	for (const auto hitbox : hitboxes)
	{
		const auto hitboxComponent = hitbox->getComponent<HitboxComponent>();
		hitboxComponent->setEnableDraw(Game::constants->DRAW_HITBOXES);
	}

	for (const auto weapon : weapons)
	{
		const auto weaponComponent = weapon->getComponent<WeaponComponent>();
		weaponComponent->setEnableDraw(Game::constants->DRAW_HITBOXES);
	}

	for (auto hitboxEntity : hitboxes)
	{
		auto hitboxCollider = hitboxEntity->getComponent<HitboxComponent>();
		auto hitboxFaction = hitboxEntity->getComponent<FactionComponent>();

		for (auto weaponWieldingEntity : weapons)
		{
			// check if hits itself
			if (hitboxEntity->getID() == weaponWieldingEntity->getID())
				continue;

			// check if hits friend
			auto weaponWielderFaction = weaponWieldingEntity->getComponent<FactionComponent>();
			if (hitboxFaction && weaponWielderFaction && hitboxFaction->checkIfFactionFriendly(weaponWielderFaction->getFaction()))
				continue;

			auto weaponCollider = weaponWieldingEntity->getComponent<WeaponComponent>();

			if (!weaponCollider->isEnabled())
				continue;
			if (weaponCollider->isInAffectedTargets(hitboxEntity->getID()))
				continue;

			if (weaponCollider->getCollider()->collidesWith(hitboxCollider->getHitbox()))
			{
				weaponCollider->addAffectedTarget(hitboxEntity->getID());
				auto actorHealthComponent = hitboxEntity->getComponent<HealthComponent>();
				auto actorArmorComponent = hitboxEntity->getComponent<ArmorComponent>();

				const auto weaponDamage = weaponCollider->getDamage();
				const auto damage = actorArmorComponent ? actorArmorComponent->applyDamageReduction(weaponDamage) : weaponDamage;
				actorHealthComponent->changeHealth(-damage);
				const int currentHealth = actorHealthComponent->getHealth();

				// =========== LOGGING ===========
				auto entityName = weaponCollider->getTag() + "_" + std::to_string(hitboxEntity->getID());
				if (hitboxEntity->hasComponent<ActorComponent>())
				{
					const auto actorComponent = hitboxEntity->getComponent<ActorComponent>();
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

void HitboxWeaponCollisionSystem::draw()
{
}