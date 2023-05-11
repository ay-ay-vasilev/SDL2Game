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
	if (armorSlots[slotName].armorName == armorName) return;
	if (armorSlots.count(slotName)) unequipArmorFromSlot(slotName);

	ArmorPiece armorPiece{ armorName, {}, 0 };

	const auto armorData = Game::assets->getArmorJson(armorName, actorName, slotName);
	const auto slotArmorValue = armorData.value("damage_reduction", 0);
	armorValue += slotArmorValue;
	armorPiece.armorVal = slotArmorValue;

	const auto& spriteData = armorData.value("sprite_data", nlohmann::json{});


	std::vector<std::pair<std::string, std::shared_ptr<Sprite>>> spritesToAdd;
	const auto& armorSpriteData = spriteData.value("sprites", nlohmann::json::array());

	for (const auto& data : armorSpriteData)
	{
		const auto& surfaceName = data.value("texture", "");
		const int z = data.value("z", 0);
		armorPiece.spriteSlots.emplace_back(data["slot"]);
		spritesToAdd.push_back({data["slot"], std::make_shared<Sprite>(surfaceName, z)});
	}
	armorSlots[slotName] = armorPiece;

	std::unordered_map<std::string, std::vector<std::string>> blockedSlots;
	const auto& armorSpriteBlockData = spriteData.value("block_slots", nlohmann::json::array());
	for (const auto& blockedSlotData : armorSpriteBlockData)
	{
		const std::string blockerSlotName = blockedSlotData["blocker_slot"];
		std::vector<std::string> blockedSlotNames;
		for (const auto& blockedSlotName : blockedSlotData["blocked_slots"]) blockedSlotNames.push_back(blockedSlotName);
		blockedSlots[blockerSlotName] = blockedSlotNames;
	}

	for (const auto [spriteSlotName, sprite] : spritesToAdd)
		spriteComponent->addSprite(spriteSlotName, sprite);

	for (const auto& [blockerSlotName, blockedSlotNames] : blockedSlots)
		spriteComponent->addBlockedSlots(blockerSlotName, blockedSlotNames);

	if (armorSpriteData.empty()) spriteComponent->removeSpritesFromSlot(slotName);
}

void ecs::ArmorComponent::unequipArmorFromSlot(const std::string& slotName)
{
	const auto armorPiece = armorSlots[slotName];

	armorValue -= armorPiece.armorVal;
	for (const auto& spriteSlot : armorPiece.spriteSlots)
	{
		spriteComponent->removeSpritesFromSlot(spriteSlot);
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
			spriteComponent->removeSpritesFromSlot(spriteSlot);
		}
	}
	armorSlots.clear();
}

const int ecs::ArmorComponent::applyDamageReduction(int damage) const
{
	return std::clamp(damage - armorValue, 0, damage);
}