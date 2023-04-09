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
		const auto frameWidth = spriteData.value("frame_width", 0);
		const auto frameHeight = spriteData.value("frame_height", 0);

		std::vector<std::shared_ptr<Sprite>> spritesToAdd;
		const auto& armorSpriteData = spriteData.value("sprites", nlohmann::json::array());
		for (const auto& data : armorSpriteData)
		{
			const auto& textureName = data.value("texture", "");
			const int spriteFrameWidth = data.value("frame_width", frameWidth);
			const int spriteFrameHeight = data.value("frame_height", frameHeight);
			const int z = data.value("z", 0);
			spritesToAdd.emplace_back(std::make_shared<Sprite>(textureName, spriteFrameWidth, spriteFrameHeight, z));
		}
		spriteComponent->addSpritesToSlot(slotName, spritesToAdd);

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

private:
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Sprite>>> tempSprites;
	std::unordered_map<std::string, int> slotArmorValues;

	int armorValue;
};