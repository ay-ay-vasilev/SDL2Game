#include "ArmorComponent.h"
#include "Game.h"
#include "AssetManager.h"

ecs::ArmorComponent::ArmorComponent() : armorValue(0) {}

void ecs::ArmorComponent::init()
{
	spriteComponent = entity->getComponent<SpriteComponent>();
}

void ecs::ArmorComponent::equipArmorToSlot(const std::string& armorName, const std::string& actorName, const std::string& slotName)
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
		const auto& surfaceName = data.value("texture", "");
		const int z = data.value("z", 0);

		SDL_Color test0 = { 165, 131, 112, 255 };
		SDL_Color test1 = { 102, 57, 49, 255 };

		SDL_Color test2 = { 131, 98, 80, 255 };
		SDL_Color test3 = { 78, 47, 51, 255 };

		SDL_Color test4 = { 251, 242, 54, 255 };
		SDL_Color test5 = { 255, 0, 0, 255 };

		SDL_Color test6 = { 246, 223, 86, 255 };
		SDL_Color test7 = { 66, 255, 248, 255 };

		SDL_Color test8 = { 69, 50, 41, 255 };
		SDL_Color test9 = { 56, 56, 55, 255 };

		std::vector<std::pair<SDL_Color, SDL_Color>> colorMapping = { {test0, test1}, {test2, test3}, {test4, test5}, {test6, test7}, {test8, test9} };

		spritesToAdd.emplace_back(std::make_shared<Sprite>(surfaceName, z, colorMapping));
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

void ecs::ArmorComponent::unequipArmorFromSlot(const std::string& slotName)
{
	armorValue -= slotArmorValues[slotName];
	slotArmorValues.erase(slotName);
	spriteComponent->removeSpritesFromSlot(slotName);
}

void ecs::ArmorComponent::unequipAllArmor()
{
	armorValue = 0;
	for (const auto [slotArmor, slotArmorValue] : slotArmorValues)
	{
		spriteComponent->removeSpritesFromSlot(slotArmor);
	}
	slotArmorValues.clear();
}

const int ecs::ArmorComponent::applyDamageReduction(int damage) const
{
	return std::clamp(damage - armorValue, 0, damage);
}