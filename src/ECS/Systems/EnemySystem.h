#pragma once
#include "ECS.h"

namespace ecs
{
	class EnemySystem : public System
	{
	public:
		explicit EnemySystem(Manager& manager) : System(manager) {}

		Entity* instantiateEnemy(const Vector2D& pos, const std::string& filename);
		void equipWeapon(Entity& enemy, const std::string& weaponName);

		void equipArmor(Entity& enemy, const std::string& armorName, const std::string& slotName);
		void unequipArmorPiece(Entity& enemy, const std::string& slotName);
		void unequipAllArmor(Entity& enemy);

		void update() override;
		void draw() override;

	private:
		std::vector<Entity*> enemies;
	};
}