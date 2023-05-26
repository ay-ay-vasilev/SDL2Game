#pragma once
#include "ECS.h"

namespace ecs
{
	class ActorSystem : public System
	{
	public:
		explicit ActorSystem(Manager& manager) : System(manager) {}

		struct ActorCustomizationSprite
		{
			std::string slotName;
			std::vector<std::string> availableSurfaces;
			int z;
		};

		// System
		void init() override;
		void update(double delta) override;
		void draw() override;

		Entity* instantiateActor(const Vector2D& pos, const std::string& filename);
		Entity* instantiatePlayer(const Vector2D& pos, const std::string& filename);

		void equipWeapon(Entity& actor, const std::string& weaponName);

		void equipArmor(Entity& actor, const std::string& armorName, const std::string& slotName, const std::string& colorName);
		void unequipArmorPiece(Entity& actor, const std::string& slotName);
		void unequipAllArmor(Entity& actor);

		void equipRandomArmor(Entity& actor);
		void equipRandomWeapon(Entity& actor);
		void addRandomCustomization(Entity& actor);

	private:
		std::vector<Entity*> actors;

		// Data structure for possible customization options for actors
		// actor name, sprite group, vector of sprites
		std::unordered_map<std::string, std::unordered_map<std::string, std::vector<ActorCustomizationSprite>>> actorCustomizations;
		// actor name, sprite group, vector of available colors
		std::unordered_map <std::string, std::unordered_map<std::string, std::vector<std::string>>> actorCustomizationColors;


		// Data structure for possible armor for actors
		// actor name, armor slot name, armor piece name, vector of available colors
		std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>> actorArmors;
		// Data structure for possible weapons for actors
		std::unordered_map<std::string, std::vector<std::string>> actorWeapons;
	};
}