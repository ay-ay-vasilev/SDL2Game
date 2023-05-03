#pragma once
#include "ECS.h"

namespace ecs
{
	class PlayerSystem : public System
	{
	public:
		explicit PlayerSystem(Manager& manager) : System(manager), lastPlayerPosition(0, 0) {}

		Entity* instantiatePlayer(const Vector2D& pos, const std::string& filename);
		void equipWeapon(Entity& player, const std::string& weaponName);
		
		void equipArmor(Entity& player, const std::string& armorName, const std::string& slotName);
		void unequipArmor(Entity& player, const std::string& slotName);

		const Vector2D getPlayerPosition();

		void update(double delta) override;
		void draw() override;

	private:
		std::vector<Entity*> players;
		Vector2D lastPlayerPosition;
	};
}
