#pragma once
#include "SpriteComponent.h"

class ArmorComponent : public Component
{
public:
	ArmorComponent() : damageReduction(0) {}

	~ArmorComponent() {}

	void init() override
	{
		spriteComponent = entity->getComponent<SpriteComponent>();
	}

	void equipArmorElement(const std::string& armorName, const std::string& actorName, const std::string& slotName)
	{
		const auto armorData = Game::assets->getArmorJson(armorName, actorName, slotName);

		damageReduction += armorData.value("damage_reduction", 0);
		if (armorData.contains("sprite_data"))
		{
			const auto armorSpriteData = armorData["sprite_data"]["sprites"];
			const auto armorFrameWidth = armorData["sprite_data"].value("frame_width", 0);
			const auto armorFrameHeight = armorData["sprite_data"].value("frame_height", 0);

			for (const auto& data : armorSpriteData)
			{
				const auto spriteFrameWidth = data.value("frame_width", armorFrameWidth);
				const auto spriteFrameHeight = data.value("frame_height", armorFrameHeight);
				const auto tempZ = data.value("z", 0);
				tempSprites[data["slot"]].emplace_back((std::make_shared<Sprite>(data["texture"], spriteFrameWidth, spriteFrameHeight, tempZ)));
			}
		}

		if (tempSprites.empty())
			spriteComponent->removeSpritesFromSlot(slotName);
		for (auto& [slotName, tempSpriteVec] : tempSprites)
		{
			spriteComponent->removeSpritesFromSlot(slotName);
			for (auto& tempSprite : tempSpriteVec)
			{
				spriteComponent->addSprite(slotName, tempSprite);
			}
		}
		tempSprites.clear();
	}

	void update() {}

private:
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Sprite>>> tempSprites;

	int damageReduction;
};