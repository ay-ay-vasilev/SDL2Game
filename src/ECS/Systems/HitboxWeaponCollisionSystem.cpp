#include "HitboxWeaponCollisionSystem.h"
#include "Collision.h"
#include "Vector2D.h"

void HitboxWeaponCollisionSystem::update()
{
	hitboxes = manager.getEntitiesWithComponent<HitboxComponent>();
	weapons = manager.getEntitiesWithComponent<WeaponComponent>();
	for (auto entity : hitboxes)
	{
		entity->getComponent<HitboxComponent>().setDebugDraw(manager.getConstants()->DRAW_HITBOXES);
	}

	for (auto entity : weapons)
	{
		entity->getComponent<WeaponComponent>().setDebugDraw(manager.getConstants()->DRAW_HITBOXES);
	}

	for (auto hitboxEntity : hitboxes)
	{
		auto& hitboxCollider = hitboxEntity->getComponent<HitboxComponent>();

		for (auto weaponWieldingEntity : weapons)
		{
			auto& weaponCollider = weaponWieldingEntity->getComponent<WeaponComponent>();
			
			if (!weaponCollider.isEnabled())
				continue;
			if (weaponCollider.getOwnerTag() == hitboxCollider.getTag())
				continue;
			if (weaponCollider.isInAffectedTargets(hitboxCollider.getId()))
				continue;

			if (weaponCollider.getCollider()->collidesWith(*(hitboxCollider.getHitbox())))
			{
				weaponCollider.addAffectedTarget(hitboxCollider.getId());
				auto& actorHealthComponent = hitboxEntity->getComponent<HealthComponent>();

				const int damage = weaponCollider.getDamage();
				actorHealthComponent.changeHealth(-damage);
				const int currentHealth = actorHealthComponent.getHealth();

				std::cout
					<< hitboxCollider.getTag() << " got hit by " << weaponCollider.getTag() << " for " << damage << " damage!"
					<< hitboxCollider.getTag() << " has " << currentHealth << " hp left!\n";

				if (currentHealth <= 0)
				{
					// TODO better death handling!
					hitboxEntity->destroy();
					std::cout << hitboxCollider.getTag() << " died!\n";
				}

				if (weaponCollider.isDestroyedOnHit())
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