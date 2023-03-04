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

	for (auto hitbox : hitboxes)
	{
		auto& hitboxCollider = hitbox->getComponent<HitboxComponent>();

		for (auto weapon : weapons)
		{
			auto& weaponCollider = weapon->getComponent<WeaponComponent>();
			
			if (!weaponCollider.isEnabled())
				continue;
			if (weaponCollider.getOwnerTag() == hitboxCollider.getTag())
				continue;
			if (weaponCollider.isInAffectedTargets(hitboxCollider.getId()))
				continue;

			if (weaponCollider.getCollider()->collidesWith(*(hitboxCollider.getHitbox())))
			{
				weaponCollider.addAffectedTarget(hitboxCollider.getId());
				std::cout << hitboxCollider.getTag() << " got hit by " << weaponCollider.getTag() << "!\n";
			}
		}
	}
}

void HitboxWeaponCollisionSystem::draw()
{
}