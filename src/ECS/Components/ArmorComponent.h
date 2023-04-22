#pragma once
#include "SpriteComponent.h"

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
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Sprite>>> tempSprites;
	std::unordered_map<std::string, int> slotArmorValues;

	int armorValue;
};