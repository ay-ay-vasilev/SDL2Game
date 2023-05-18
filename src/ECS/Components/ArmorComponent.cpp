#include "ArmorComponent.h"
#include "Game.h"
#include "AssetManager.h"
#include "ActorComponent.h"

ecs::ArmorComponent::ArmorComponent() : armorValue(0) {}

void ecs::ArmorComponent::init()
{
	actorComponent = entity->getComponent<ecs::ActorComponent>();
}

void ecs::ArmorComponent::equipArmorToSlot(const std::string& armorName, const std::string& actorName, const std::string& slotName, const std::string& colorName)
{
	if (armorSlots[slotName].armorName == armorName) return;
	if (armorSlots.count(slotName)) unequipArmorFromSlot(slotName);

	ArmorPiece armorPiece{ armorName, {}, 0};

	const auto armorData = assets::getArmorJson(armorName, actorName, slotName);

	const auto slotArmorValue = armorData.value("damage_reduction", 0);
	armorValue += slotArmorValue;
	armorPiece.armorVal = slotArmorValue;

	const auto& spriteData = armorData.value("sprite_data", nlohmann::json{});
	const auto& armorSpriteData = spriteData.value("sprites", nlohmann::json::array());
	const auto& armorColorsData = spriteData.value("colors", nlohmann::json::array());
	bool hasColor = find(armorColorsData.begin(), armorColorsData.end(), colorName) != armorColorsData.end();
	if (hasColor) armorPiece.color = colorName;
	else if (colorName != "default")
	{
		std::cout << "Color " << colorName << " not found for armor piece " << armorName << " for actor " << actorName << " in slot " << slotName << "!\n";
	}

	for (const auto& data : armorSpriteData)
	{
		const auto& surfaceName = data.value("texture", "");
		const int z = data.value("z", 0);
		armorPiece.spriteSlots.emplace_back(data["slot"]);
		if (hasColor) actorComponent->addSprite(data["slot"], surfaceName, z, colorName);
		else actorComponent->addSprite(data["slot"], surfaceName, z);
	}
	armorSlots[slotName] = armorPiece;

	const auto& armorSpriteBlockData = spriteData.value("block_slots", nlohmann::json::array());
	for (const auto& blockedSlotData : armorSpriteBlockData)
	{
		const std::string blockerSlotName = blockedSlotData["blocker_slot"];
		std::vector<std::string> blockedSlotNames;
		for (const auto& blockedSlotName : blockedSlotData["blocked_slots"]) blockedSlotNames.push_back(blockedSlotName);
		actorComponent->addBlockedSlots(blockerSlotName, blockedSlotNames);
	}

	if (armorSpriteData.empty()) actorComponent->removeSpritesFromSlot(slotName);
}

void ecs::ArmorComponent::unequipArmorFromSlot(const std::string& slotName)
{
	const auto armorPiece = armorSlots[slotName];

	armorValue -= armorPiece.armorVal;
	for (const auto& spriteSlot : armorPiece.spriteSlots)
	{
		actorComponent->removeSpritesFromSlot(spriteSlot);
	}
	armorSlots.erase(slotName);
}

void ecs::ArmorComponent::unequipAllArmor()
{
	armorValue = 0;
	for (const auto [armorSlotName, armorPiece] : armorSlots)
	{
		for (const auto& spriteSlot : armorPiece.spriteSlots)
		{
			actorComponent->removeSpritesFromSlot(spriteSlot);
		}
	}
	armorSlots.clear();
}

const int ecs::ArmorComponent::applyDamageReduction(int damage) const
{
	return std::clamp(damage - armorValue, 1, damage);
}