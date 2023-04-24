#include "HitboxWeaponCollisionSystem.h"
#include "HitboxComponent.h"
#include "WeaponComponent.h"
#include "HealthComponent.h"
#include "ArmorComponent.h"
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

		for (auto weaponWieldingEntity : weapons)
		{
			auto weaponCollider = weaponWieldingEntity->getComponent<WeaponComponent>();
			
			if (!weaponCollider->isEnabled())
				continue;
			if (weaponCollider->getOwnerTag() == hitboxCollider->getTag())
				continue;
			if (weaponCollider->isInAffectedTargets(hitboxCollider->getId()))
				continue;

			if (weaponCollider->getCollider()->collidesWith(hitboxCollider->getHitbox()))
			{
				weaponCollider->addAffectedTarget(hitboxCollider->getId());
				auto actorHealthComponent = hitboxEntity->getComponent<HealthComponent>();
				auto actorArmorComponent = hitboxEntity->getComponent<ArmorComponent>();

				const auto weaponDamage = weaponCollider->getDamage();
				const auto damage = actorArmorComponent ? actorArmorComponent->applyDamageReduction(weaponDamage) : weaponDamage;
				actorHealthComponent->changeHealth(-damage);
				const int currentHealth = actorHealthComponent->getHealth();

				std::cout
					<< hitboxCollider->getTag() << " got hit by " << weaponCollider->getTag() << " for " << damage << " damage!"
					<< hitboxCollider->getTag() << " has " << currentHealth << " hp left!\n";

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