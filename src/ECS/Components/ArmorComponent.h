#pragma once
#include "SpriteComponent.h"

class ArmorComponent : public Component
{
public:
	ArmorComponent() : armorValue(0) {}

	~ArmorComponent() {}

	void init() override
	{
		spriteComponent = entity->getComponent<SpriteComponent>();
	}

	void equipArmorToSlot(const std::string& armorName, const std::string& actorName, const std::string& slotName)
	{
		const auto armorData = Game::assets->getArmorJson(armorName, actorName, slotName);

		const auto slotArmorValue = armorData.value("damage_reduction", 0);
		armorValue += slotArmorValue;
		slotArmorValues[slotName] = slotArmorValue;

		const auto& spriteData = armorData.value("sprite_data", nlohmann::json{});

		std::vector<std::shared_ptr<Sprite>> spritesToAdd;
		const auto& armorSpriteData = spriteData.value("sprites", nlohmann::json::array());
		for (const auto& data : armorSpriteData)
		{
			const auto& textureName = data.value("texture", "");
			const int z = data.value("z", 0);
			spritesToAdd.emplace_back(std::make_shared<Sprite>(textureName, z));
		}

		std::unordered_map<std::string, std::vector<std::string>> blockedSlots;
		const auto& armorSpriteBlockData = spriteData.value("block_slots", nlohmann::json::array());
		for (const auto& blockedSlotData : armorSpriteBlockData)
		{
			const std::string blockerSlotName = blockedSlotData["blocker_slot"];
			std::vector<std::string> blockedSlotNames;
			for (const auto& blockedSlotName : blockedSlotData["blocked_slots"])
				blockedSlotNames.push_back(blockedSlotName);
			blockedSlots[blockerSlotName] = blockedSlotNames;
		}

		spriteComponent->addSpritesToSlot(slotName, spritesToAdd);
		for (const auto& [blockerSlotName, blockedSlotNames] : blockedSlots)
		{
			spriteComponent->addBlockedSlots(blockerSlotName, blockedSlotNames);
		}

		if (armorSpriteData.empty()) {
			spriteComponent->removeSpritesFromSlot(slotName);
		}
	}

	void unequipArmorFromSlot(const std::string& slotName)
	{
		armorValue -= slotArmorValues[slotName];
		slotArmorValues.erase(slotName);
		spriteComponent->removeSpritesFromSlot(slotName);
	}

	void unequipAllArmor()
	{
		armorValue = 0;
		for (const auto [slotArmor, slotArmorValue] : slotArmorValues)
		{
			spriteComponent->removeSpritesFromSlot(slotArmor);
		}
		slotArmorValues.clear();
	}

	void update() {}

	int applyDamageReduction(int damage) const
	{
		return std::clamp(damage - armorValue, 0, damage);
	}

private:
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Sprite>>> tempSprites;
	std::unordered_map<std::string, int> slotArmorValues;

	int armorValue;
};