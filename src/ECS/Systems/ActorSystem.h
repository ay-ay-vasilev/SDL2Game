#pragma once
#include "ECS.h"

namespace ecs
{
	class ActorSystem : public System
	{
	public:
		explicit ActorSystem(Manager& manager) : System(manager) {}

		Entity* instantiateActor(const Vector2D& pos, const std::string& filename);
		void equipWeapon(Entity& actor, const std::string& weaponName);

		void equipArmor(Entity& actor, const std::string& armorName, const std::string& slotName);
		void unequipArmorPiece(Entity& actor, const std::string& slotName);
		void unequipAllArmor(Entity& actor);

		void update() override;
		void draw() override;

	private:
		std::vector<Entity*> actors;

		// Data structure for possible equipment for actors
		std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> actorEquipment;



	};
}