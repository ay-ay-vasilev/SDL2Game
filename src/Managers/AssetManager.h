#pragma once
#include "TextureManager.h"
#include "Vector2D.h"
#include <SDL_ttf.h>
#include <wrappedJson.h>

#include <map>
#include <string>
#include <filesystem>

namespace ecs
{
	class Manager;
}

namespace assets
{
	std::filesystem::path getDataPath();
	std::string getActorJsonPath(const std::string fileName);
	std::string getProjectileJsonPath(const std::string fileName);
	std::string getWeaponJsonPath(const std::string fileName);
	std::string getArmorJsonPath(const std::string fileName, const std::string actorName, const std::string slotName);
	std::string getGeneralDataJsonPath(const std::string fileName);
	std::string getAssetDataJsonPath(const std::string fileName);
	std::string getParticleDataJsonPath(const std::string fileName);
	nlohmann::json getJsonData(const std::string filePath);
	nlohmann::json getActorJson(const std::string actorName);
	nlohmann::json getProjectileJson(const std::string projectileName);
	nlohmann::json getWeaponJson(const std::string weaponName);
	nlohmann::json getArmorJson(const std::string armorName, const std::string actorName, const std::string slotName);
	nlohmann::json getGeneralDataJson(const std::string dataName);
	nlohmann::json getAssetDataJson(const std::string assetDataName);
	nlohmann::json getParticleDataJson(const std::string particleDataName);
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
		void loadActorSurfaces();
		void loadMapSurfaces();
		void loadMiscSurfaces();

		void loadColorVariationsForSurface(const std::string textureName, const std::string texturePath, nlohmann::json colorVariationsData);
		void loadFonts();

		// Surface management
		void addSurface(const std::string id, const std::string path, const std::string color = "default");
		SDL_Surface* getSurface(const std::string id, const std::string color = "default") const;

		// Font management
		void addFont(const std::string id, const std::string path, const int fontSize);
		TTF_Font* getFont(const std::string id) const;

	private:
		std::shared_ptr<ecs::Manager> manager;

		// texture name, then color name, then the surface
		std::map<std::string, std::unordered_map<std::string, SDL_Surface*>> coloredSurfaces;
		// texture name, then color name, then color variation - maybe optimize into groups later ?
		std::map<std::string, std::unordered_map<std::string, std::vector<std::pair<SDL_Color, SDL_Color>>>> colorVariations;

		std::map<std::string, TTF_Font*> fonts;
	};
}