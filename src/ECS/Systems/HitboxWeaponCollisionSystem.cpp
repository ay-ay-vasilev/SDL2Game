#include "HitboxWeaponCollisionSystem.h"
#include "Collision.h"
#include "Vector2D.h"

void HitboxWeaponCollisionSystem::update()
{
	hitboxes = manager.getEntitiesWithComponent<HitboxComponent>();
	weapons = manager.getEntitiesWithComponent<WeaponComponent>();
	if (manager.getConstants()->DRAW_HITBOXES)
	{
		for (auto entity : hitboxes)
		{
			entity->getComponent<HitboxComponent>().setDebugDraw(true);
		}

		for (auto entity : weapons)
		{
			entity->getComponent<WeaponComponent>().setDebugDraw(true);
		}
	}

	for (auto hitbox : hitboxes)
	{
		auto& hitboxCollider = hitbox->getComponent<HitboxComponent>();

		for (auto weapon : weapons)
		{
			auto& weaponCollider = weapon->getComponent<WeaponComponent>();
			
			if (!weaponCollider.isEnabled)
				continue;
			if (weaponCollider.ownerTag == hitboxCollider.tag)
				continue;
			if (std::find(weaponCollider.affectedTargets.begin(), weaponCollider.affectedTargets.end(), hitboxCollider.id) != weaponCollider.affectedTargets.end())
				continue;

			if (Collision::AABB(weaponCollider.weaponCollider, hitboxCollider.hitbox))
			{
				weaponCollider.addAffectedTarget(hitboxCollider.id);
				std::cout << hitboxCollider.tag << " got hit by " << weaponCollider.tag << "!\n";
			}
		}
	}
}

void HitboxWeaponCollisionSystem::draw()
{
}