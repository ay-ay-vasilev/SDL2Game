#include "ECS.h"
#include "AssetManager.h"
#include "Constants.h"

AssetManager::AssetManager(std::shared_ptr<ecs::Manager> manager) : manager(manager), constants(manager->getConstants()) {}

AssetManager::~AssetManager()
{
	for (auto& pair : surfaces) {
		SDL_FreeSurface(pair.second);
	}
	surfaces.clear();
}

void AssetManager::loadSurfaces()
{
	addSurface("terrain", "assets/images/spritesheets/tiles_v0.png");

	addSurface("goblin_body", "assets/images/spritesheets/actors/goblin/body/spritesheet.png");
	addSurface("goblin_head", "assets/images/spritesheets/actors/goblin/head/spritesheet.png");
	addSurface("goblin_face_0", "assets/images/spritesheets/actors/goblin/head/face/spritesheet_0.png");
	addSurface("goblin_ears_0", "assets/images/spritesheets/actors/goblin/head/ears/spritesheet_0.png");
	addSurface("goblin_hands_0", "assets/images/spritesheets/actors/goblin/hands/spritesheet_0.png");
	addSurface("goblin_hands_1", "assets/images/spritesheets/actors/goblin/hands/spritesheet_1.png");

	addSurface("goblin_pants_brown", "assets/images/spritesheets/actors/goblin/armor/pants/pants_brown.png");
	addSurface("goblin_pants_light", "assets/images/spritesheets/actors/goblin/armor/pants/pants_light.png");
	addSurface("goblin_shirt_light", "assets/images/spritesheets/actors/goblin/armor/shirt/shirt_light.png");
	addSurface("goblin_long_sleeve_light", "assets/images/spritesheets/actors/goblin/armor/shirt/long_sleeve_light.png");
	addSurface("goblin_long_sleeve_light_hands_0", "assets/images/spritesheets/actors/goblin/armor/shirt/long_sleeve_light_hands_0.png");
	addSurface("goblin_jacket_dark", "assets/images/spritesheets/actors/goblin/armor/jacket/jacket_dark.png");
	addSurface("goblin_light_jacket_brown", "assets/images/spritesheets/actors/goblin/armor/jacket/light_jacket_brown.png");
	addSurface("goblin_eyewear_eyepatch_black", "assets/images/spritesheets/actors/goblin/armor/eyewear/eyepatch_black.png");
	addSurface("goblin_mask_scarf_black", "assets/images/spritesheets/actors/goblin/armor/mask/scarf_black.png");
	addSurface("goblin_cloak_0", "assets/images/spritesheets/actors/goblin/armor/overcoat/cloak_0.png");
	addSurface("goblin_cloak_0_hands_0", "assets/images/spritesheets/actors/goblin/armor/overcoat/cloak_0_hands_0.png");
	addSurface("goblin_cloak_0_head", "assets/images/spritesheets/actors/goblin/armor/overcoat/cloak_0_head.png");

	addSurface("human_body", "assets/images/spritesheets/actors/human/body/spritesheet.png");
	addSurface("human_head", "assets/images/spritesheets/actors/human/head/spritesheet.png");
	addSurface("human_face_0", "assets/images/spritesheets/actors/human/head/face/spritesheet_0.png");
	addSurface("human_hair_3", "assets/images/spritesheets/actors/human/head/hair/3.png");
	addSurface("human_hands_0", "assets/images/spritesheets/actors/human/hands/spritesheet_0.png");
	addSurface("human_hands_1", "assets/images/spritesheets/actors/human/hands/spritesheet_1.png");

	addSurface("human_pants_brown", "assets/images/spritesheets/actors/human/armor/pants/pants_brown.png");
	addSurface("human_shirt_light", "assets/images/spritesheets/actors/human/armor/shirt/shirt_light.png");
	addSurface("human_shirt_light_hands_back", "assets/images/spritesheets/actors/human/armor/shirt/shirt_light_hands_0.png");
	addSurface("human_shirt_light_hands_front", "assets/images/spritesheets/actors/human/armor/shirt/shirt_light_hands_1.png");
	addSurface("human_chainmail", "assets/images/spritesheets/actors/human/armor/jacket/chainmail.png");
	addSurface("human_chainmail_hands_back", "assets/images/spritesheets/actors/human/armor/jacket/chainmail_hands_back.png");
	addSurface("human_chainmail_hands_front", "assets/images/spritesheets/actors/human/armor/jacket/chainmail_hands_front.png");
	addSurface("human_surcoat", "assets/images/spritesheets/actors/human/armor/overcoat/surcoat.png");
	addSurface("human_surcoat_hands", "assets/images/spritesheets/actors/human/armor/overcoat/surcoat_hands.png");
	addSurface("human_eyepatch_black", "assets/images/spritesheets/actors/human/armor/eyewear/eyepatch_black.png");
	addSurface("human_scarf_black", "assets/images/spritesheets/actors/human/armor/mask/scarf_black.png");
	addSurface("human_helmet", "assets/images/spritesheets/actors/human/armor/headwear/helmet.png");

	addSurface("skeleton_body", "assets/images/spritesheets/actors/skeleton/body/spritesheet.png");
	addSurface("skeleton_head_0", "assets/images/spritesheets/actors/skeleton/head/spritesheet_0.png");
	addSurface("skeleton_head_1", "assets/images/spritesheets/actors/skeleton/head/spritesheet_1.png");
	addSurface("skeleton_hands_0", "assets/images/spritesheets/actors/skeleton/hands/spritesheet_0.png");
	addSurface("skeleton_hands_1", "assets/images/spritesheets/actors/skeleton/hands/spritesheet_1.png");

	addSurface("skeleton_pants_brown", "assets/images/spritesheets/actors/skeleton/armor/pants/pants_brown.png");
	addSurface("skeleton_shirt_torn_gray", "assets/images/spritesheets/actors/skeleton/armor/shirt/shirt_torn_gray.png");
	addSurface("skeleton_chainmail", "assets/images/spritesheets/actors/skeleton/armor/jacket/chainmail.png");
	addSurface("skeleton_chainmail_hands_back", "assets/images/spritesheets/actors/skeleton/armor/jacket/chainmail_hands_back.png");
	addSurface("skeleton_chainmail_hands_front", "assets/images/spritesheets/actors/skeleton/armor/jacket/chainmail_hands_front.png");
	addSurface("skeleton_heavy_armor", "assets/images/spritesheets/actors/skeleton/armor/armorplate/heavy_armor.png");
	addSurface("skeleton_heavy_armor_hands_back", "assets/images/spritesheets/actors/skeleton/armor/armorplate/heavy_armor_hands_back.png");
	addSurface("skeleton_heavy_armor_hands_front", "assets/images/spritesheets/actors/skeleton/armor/armorplate/heavy_armor_hands_front.png");
	addSurface("skeleton_heavy_helmet", "assets/images/spritesheets/actors/skeleton/armor/headwear/heavy_helmet.png");

	addSurface("shortsword_goblin_0", "assets/images/spritesheets/weapons/melee_light/shortsword/goblin/spritesheet_0.png");
	addSurface("shortsword_goblin_1", "assets/images/spritesheets/weapons/melee_light/shortsword/goblin/spritesheet_1.png");

	addSurface("sword_goblin_0", "assets/images/spritesheets/weapons/melee_medium/sword/goblin/spritesheet_0.png");
	addSurface("sword_human_0", "assets/images/spritesheets/weapons/melee_medium/sword/human/spritesheet_0.png");
	addSurface("sword_human_1", "assets/images/spritesheets/weapons/melee_medium/sword/human/spritesheet_1.png");
	addSurface("sword_skeleton_0", "assets/images/spritesheets/weapons/melee_medium/sword/skeleton/spritesheet_0.png");
	addSurface("sword_skeleton_1", "assets/images/spritesheets/weapons/melee_medium/sword/skeleton/spritesheet_1.png");

	addSurface("test_projectile", "assets/images/misc/test_projectile.png");
	addSurface("black", "assets/images/misc/black.png");
}

void AssetManager::loadFonts()
{
	addFont("arial", "../assets/fonts/arial.ttf", constants->DEBUG_FONT_SIZE);
}

void AssetManager::addSurface(const std::string_view& id, const std::string& path)
{
	surfaces.emplace(id, TextureManager::loadSurface(path));
}

SDL_Surface* AssetManager::getSurface(const std::string_view& id) const
{
	auto it = surfaces.find(id);
	if (it != surfaces.end())
	{
		return it->second;
	}
	else
	{
		std::cout << "Error: texture not found (" << id << ").\n";
		return nullptr;
	}
}

void AssetManager::addFont(const std::string_view& id, const std::string& path, const int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::getFont(const std::string_view& id) const
{
	auto it = fonts.find(id);
	if (it != fonts.end())
	{
		return it->second;
	}
	else
	{
		std::cout << "Error: font not found (" << id << ").\n";
		return nullptr;
	}
}

std::filesystem::path AssetManager::getDataPath()
{
	return std::filesystem::current_path() / ".." / "data";
}

std::string AssetManager::getActorJsonPath(const std::string& fileName)
{
	std::filesystem::path actorPath = getDataPath() / "actors" / (fileName + ".json");
	return actorPath.string();
}

std::string AssetManager::getProjectileJsonPath(const std::string& fileName)
{
	std::filesystem::path projectilePath = getDataPath() / "projectiles" / (fileName + ".json");
	return projectilePath.string();
}

std::string AssetManager::getWeaponJsonPath(const std::string& fileName)
{
	std::filesystem::path weaponPath = getDataPath() / "weapons" / (fileName + ".json");
	return weaponPath.string();
}

std::string AssetManager::getArmorJsonPath(const std::string& fileName, const std::string& actorName, const std::string& slotName)
{
	std::filesystem::path armorPath = getDataPath() / "armor" / actorName / slotName / (fileName + ".json");
	return armorPath.string();
}

std::string AssetManager::getGeneralDataJsonPath(const std::string& fileName)
{
	std::filesystem::path generalPath = getDataPath() / "general" / (fileName + ".json");
	return generalPath.string();
}

nlohmann::json AssetManager::getJsonData(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + filePath);
	}
	nlohmann::json jsonData;
	file >> jsonData;
	return jsonData;
}

nlohmann::json AssetManager::getActorJson(const std::string& actorName)
{
	const std::string filePath = getActorJsonPath(actorName);
	return getJsonData(filePath);
}

nlohmann::json AssetManager::getProjectileJson(const std::string& projectileName)
{
	const std::string filePath = getProjectileJsonPath(projectileName);
	return getJsonData(filePath);
}

nlohmann::json AssetManager::getWeaponJson(const std::string& weaponName)
{
	const std::string filePath = getWeaponJsonPath(weaponName);
	return getJsonData(filePath);
}

nlohmann::json AssetManager::getArmorJson(const std::string& armorName, const std::string& actorName, const std::string& slotName)
{
	const std::string filePath = getArmorJsonPath(armorName, actorName, slotName);
	return getJsonData(filePath);
}

nlohmann::json AssetManager::getGeneralDataJson(const std::string& dataName)
{
	const std::string filePath = getGeneralDataJsonPath(dataName);
	return getJsonData(filePath);
}