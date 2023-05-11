#pragma once
#include "SpriteComponent.h"

namespace ecs
{
	class ArmorComponent : public Component
	{
	public:
		ArmorComponent();
		~ArmorComponent() {}

		// Component
		void init() override;

		void equipArmorToSlot(const std::string& armorName, const std::string& actorName, const std::string& slotName);
		void unequipArmorFromSlot(const std::string& slotName);
		void unequipAllArmor();

		const int applyDamageReduction(int damage) const;

	private:
		struct ArmorPiece
		{
			// TODO: improve the structure eventually
			std::string armorName;						// example: dark_robes
			std::vector<std::string> spriteSlots;		// example: overcoat, hood
			int armorVal;								// example: 5
		};

		std::shared_ptr<SpriteComponent> spriteComponent;
		std::unordered_map<std::string, ArmorPiece> armorSlots; // *key* - armor slot, *value* - armor piece on that slot

		int armorValue;
	};
}