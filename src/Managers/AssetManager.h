#pragma once
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include <SDL_ttf.h>

#include <map>
#include <string>
#include <filesystem>

class Constants;
class AssetManager
{
public:
	AssetManager(std::shared_ptr<Manager> manager);
	~AssetManager();

	void loadTextures();
	void loadFonts();
	
	std::filesystem::path getDataPath();
	std::string getActorJsonPath(const std::string& fileName);
	std::string getProjectileJsonPath(const std::string& fileName);
	std::string getWeaponJsonPath(const std::string& fileName);
	std::string getArmorJsonPath(const std::string& fileName, const std::string& actorName, const std::string& slotName);

	// Texture management
	void addTexture(const std::string_view& id, const std::string& path);
	SDL_Texture* getTexture(const std::string_view& id) const;

	// Font management
	void addFont(const std::string_view& id, const std::string& path, const int fontSize);
	TTF_Font* getFont(const std::string_view& id) const;

	nlohmann::json getJsonData(const std::string& filePath);
	nlohmann::json getActorJson(const std::string& actorName);
	nlohmann::json getProjectileJson(const std::string& projectileName);
	nlohmann::json getWeaponJson(const std::string& weaponName);
	nlohmann::json getArmorJson(const std::string& armorName, const std::string& actorName, const std::string& slotName);

private:

	std::shared_ptr<Manager> manager;
	std::map<const std::string_view, SDL_Texture*> textures;
	std::map<const std::string_view, TTF_Font*> fonts;
	std::shared_ptr<Constants> constants;
};