#pragma once
#include "TextureManager.h"
#include "Vector2D.h"
#include <SDL_ttf.h>
#include "wrappedJson.h"

#include <map>
#include <string>
#include <filesystem>

namespace ecs
{
	class Manager;
}

class Constants;
class AssetManager
{
public:
	explicit AssetManager(std::shared_ptr<ecs::Manager> manager);
	~AssetManager();

	// Disallow copy
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

	// Disallow move
	AssetManager(AssetManager&&) = delete;
	AssetManager& operator=(AssetManager&&) = delete;

	void loadSurfaces();
	void loadFonts();
	
	std::filesystem::path getDataPath();
	std::string getActorJsonPath(const std::string& fileName);
	std::string getProjectileJsonPath(const std::string& fileName);
	std::string getWeaponJsonPath(const std::string& fileName);
	std::string getArmorJsonPath(const std::string& fileName, const std::string& actorName, const std::string& slotName);
	std::string getGeneralDataJsonPath(const std::string& fileName);

	// Surface management
	void addSurface(const std::string_view& id, const std::string& path);
	SDL_Surface* getSurface(const std::string_view& id) const;

	// Font management
	void addFont(const std::string_view& id, const std::string& path, const int fontSize);
	TTF_Font* getFont(const std::string_view& id) const;

	nlohmann::json getJsonData(const std::string& filePath);
	nlohmann::json getActorJson(const std::string& actorName);
	nlohmann::json getProjectileJson(const std::string& projectileName);
	nlohmann::json getWeaponJson(const std::string& weaponName);
	nlohmann::json getArmorJson(const std::string& armorName, const std::string& actorName, const std::string& slotName);
	nlohmann::json getGeneralDataJson(const std::string& dataName);

private:

	std::shared_ptr<ecs::Manager> manager;
	std::map<const std::string_view, SDL_Surface*> surfaces;
	std::map<const std::string_view, TTF_Font*> fonts;
	std::shared_ptr<Constants> constants;
};