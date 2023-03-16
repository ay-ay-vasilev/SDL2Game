#include "AssetManager.h"
#include "Constants.h"

AssetManager::AssetManager(std::shared_ptr<Manager> manager) : manager(manager), constants(manager->getConstants()) {}

AssetManager::~AssetManager() {}

void AssetManager::loadTextures()
{
	addTexture("terrain", "assets/images/spritesheets/tiles_v0.png");
	addTexture("goblin", "assets/images/spritesheets/goblin_downscale_spritesheet.png");
	addTexture("test_projectile", "assets/images/misc/test_projectile.png");
	addTexture("human", "assets/images/spritesheets/human_downscale_spritesheet.png");
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

nlohmann::json AssetManager::getActorJson(const std::string& actorName)
{
	const auto fileName = std::string(actorName);
	std::string filenameString(Game::assets->getActorJsonPath(fileName));
	std::ifstream file(filenameString);
	if (!file.is_open())
	{
		std::cerr << "Failed to open an actor file: " << filenameString << std::endl;
		return nlohmann::json();
	}
	nlohmann::json actorData;
	file >> actorData;
	return actorData;
}

nlohmann::json AssetManager::getProjectileJson(const std::string& projectileName)
{
	const auto fileName = std::string(projectileName);
	std::string filenameString(Game::assets->getProjectileJsonPath(fileName));
	std::ifstream file(filenameString);
	if (!file.is_open())
	{
		std::cerr << "Failed to open a projectile file: " << filenameString << std::endl;
		return nlohmann::json();
	}
	nlohmann::json projectileData;
	file >> projectileData;
	return projectileData;
}

nlohmann::json AssetManager::getWeaponJson(const std::string& weaponName)
{
	const auto fileName = std::string(weaponName);
	std::string filenameString(Game::assets->getWeaponJsonPath(fileName));
	std::ifstream file(filenameString);
	if (!file.is_open())
	{
		std::cerr << "Failed to open a weapon file: " << filenameString << std::endl;
		return nlohmann::json();
	}
	nlohmann::json weaponData;
	file >> weaponData;
	return weaponData;
}