#include "AssetManager.h"
#include "Constants.h"

AssetManager::AssetManager(std::shared_ptr<Manager> manager) : manager(manager), constants(manager->getConstants()) {}

AssetManager::~AssetManager() {}

void AssetManager::loadTextures()
{
	addTexture("terrain", "assets/images/spritesheets/tiles_v0.png");

	addTexture("goblin", "assets/images/spritesheets/actors/goblin/body/spritesheet.png");
	
	addTexture("goblin_hands_unarmed_0", "assets/images/spritesheets/actors/goblin/hands/melee_unarmed/spritesheet_0.png");
	addTexture("goblin_hands_unarmed_1", "assets/images/spritesheets/actors/goblin/hands/melee_unarmed/spritesheet_1.png");
	addTexture("goblin_hands_light_0", "assets/images/spritesheets/actors/goblin/hands/melee_light/spritesheet_0.png");
	addTexture("goblin_hands_light_1", "assets/images/spritesheets/actors/goblin/hands/melee_light/spritesheet_1.png");
	addTexture("goblin_hands_medium_0", "assets/images/spritesheets/actors/goblin/hands/melee_medium/spritesheet_0.png");

	addTexture("human", "assets/images/spritesheets/actors/human/body/spritesheet.png");

	addTexture("human_hands_0", "assets/images/spritesheets/actors/human/hands/melee_unarmed/spritesheet_0.png");
	addTexture("human_hands_1", "assets/images/spritesheets/actors/human/hands/melee_unarmed/spritesheet_1.png");

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