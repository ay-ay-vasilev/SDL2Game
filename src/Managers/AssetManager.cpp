#include "AssetManager.h"
#include "Constants.h"

AssetManager::AssetManager(std::shared_ptr<Manager> manager) : manager(manager), constants(manager->getConstants()) {}

AssetManager::~AssetManager() {}

void AssetManager::loadTextures()
{
	addTexture("terrain", "assets/images/spritesheets/tiles_v0.png");

	addTexture("goblin_body", "assets/images/spritesheets/actors/goblin/body/spritesheet.png");
	addTexture("goblin_head", "assets/images/spritesheets/actors/goblin/head/spritesheet.png");
	addTexture("goblin_face_0", "assets/images/spritesheets/actors/goblin/head/face/spritesheet_0.png");
	addTexture("goblin_ears_0", "assets/images/spritesheets/actors/goblin/head/ears/spritesheet_0.png");
	addTexture("goblin_hands_0", "assets/images/spritesheets/actors/goblin/hands/spritesheet_0.png");
	addTexture("goblin_hands_1", "assets/images/spritesheets/actors/goblin/hands/spritesheet_1.png");

	addTexture("goblin_pants_brown", "assets/images/spritesheets/actors/goblin/armor/pants/pants_brown.png");
	addTexture("goblin_shirt_light", "assets/images/spritesheets/actors/goblin/armor/shirt/shirt_light.png");
	addTexture("goblin_jacket_jet", "assets/images/spritesheets/actors/goblin/armor/jacket/jacket_jet.png");
	addTexture("goblin_eyewear_eyepatch_black", "assets/images/spritesheets/actors/goblin/armor/eyewear/eyepatch_black.png");
	addTexture("goblin_mask_scarf_black", "assets/images/spritesheets/actors/goblin/armor/mask/scarf_black.png");
	addTexture("goblin_cloak_0", "assets/images/spritesheets/actors/goblin/armor/overcoat/cloak_0.png");
	addTexture("goblin_cloak_0_hands_0", "assets/images/spritesheets/actors/goblin/armor/overcoat/cloak_0_hands_0.png");
	addTexture("goblin_cloak_0_head", "assets/images/spritesheets/actors/goblin/armor/overcoat/cloak_0_head.png");

	addTexture("human_body", "assets/images/spritesheets/actors/human/body/spritesheet.png");
	addTexture("human_head", "assets/images/spritesheets/actors/human/head/spritesheet.png");
	addTexture("human_face_0", "assets/images/spritesheets/actors/human/head/face/spritesheet_0.png");
	addTexture("human_hands_0", "assets/images/spritesheets/actors/human/hands/spritesheet_0.png");
	addTexture("human_hands_1", "assets/images/spritesheets/actors/human/hands/spritesheet_1.png");

	addTexture("human_pants_brown", "assets/images/spritesheets/actors/human/armor/pants/pants_brown.png");
	addTexture("human_shirt_light", "assets/images/spritesheets/actors/human/armor/shirt/shirt_light.png");
	addTexture("human_shirt_light_hands_0", "assets/images/spritesheets/actors/human/armor/shirt/shirt_light_hands_0.png");
	addTexture("human_shirt_light_hands_1", "assets/images/spritesheets/actors/human/armor/shirt/shirt_light_hands_1.png");
	addTexture("human_chainmail", "assets/images/spritesheets/actors/human/armor/jacket/chainmail.png");
	addTexture("human_chainmail_hands_back", "assets/images/spritesheets/actors/human/armor/jacket/chainmail_hands_back.png");
	addTexture("human_chainmail_hands_front", "assets/images/spritesheets/actors/human/armor/jacket/chainmail_hands_front.png");

	addTexture("shortsword_goblin_0", "assets/images/spritesheets/weapons/melee_light/shortsword/goblin/spritesheet_0.png");
	addTexture("shortsword_goblin_1", "assets/images/spritesheets/weapons/melee_light/shortsword/goblin/spritesheet_1.png");

	addTexture("sword_goblin_0", "assets/images/spritesheets/weapons/melee_medium/sword/goblin/spritesheet_0.png");
	addTexture("sword_human_0", "assets/images/spritesheets/weapons/melee_medium/sword/human/spritesheet_0.png");
	addTexture("sword_human_1", "assets/images/spritesheets/weapons/melee_medium/sword/human/spritesheet_1.png");

	addTexture("test_projectile", "assets/images/misc/test_projectile.png");
	addTexture("black", "assets/images/misc/black.png");
}

void AssetManager::loadFonts()
{
	addFont("arial", "../assets/fonts/arial.ttf", constants->DEBUG_FONT_SIZE);
}

void AssetManager::addTexture(const std::string_view& id, const std::string& path)
{
	textures.emplace(id, TextureManager::loadTexture(path));
}

SDL_Texture* AssetManager::getTexture(const std::string_view& id) const
{
	auto it = textures.find(id);
	if (it != textures.end())
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