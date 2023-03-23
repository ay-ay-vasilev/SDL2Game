#include "AssetManager.h"
#include "Constants.h"

AssetManager::AssetManager(std::shared_ptr<Manager> manager) : manager(manager), constants(manager->getConstants()) {}

AssetManager::~AssetManager() {}

void AssetManager::loadTextures()
{
	addTexture("terrain", "assets/images/spritesheets/tiles_v0.png");

	addTexture("goblin", "assets/images/spritesheets/actors/goblin/body/melee_unarmed/spritesheet.png");
	addTexture("human", "assets/images/spritesheets/actors/human/body/melee_unarmed/spritesheet.png");

	addTexture("shortsword_goblin_front", "assets/images/spritesheets/weapons/melee_light/shortsword/goblin/spritesheet_front.png");
	addTexture("shortsword_goblin_back", "assets/images/spritesheets/weapons/melee_light/shortsword/goblin/spritesheet_back.png");

	addTexture("sword_human_front", "assets/images/spritesheets/weapons/melee_medium/sword/human/spritesheet_front.png");
	addTexture("sword_human_back", "assets/images/spritesheets/weapons/melee_medium/sword/human/spritesheet_back.png");

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