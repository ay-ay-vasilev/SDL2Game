#include "ECS.h"
#include "AssetManager.h"
#include "Constants.h"

namespace assets
{
	std::filesystem::path getDataPath()
	{
		return std::filesystem::current_path() / ".." / "data";
	}

	std::string getActorJsonPath(const std::string fileName)
	{
		std::filesystem::path actorPath = getDataPath() / "actors" / (fileName + ".json");
		return actorPath.string();
	}

	std::string getProjectileJsonPath(const std::string fileName)
	{
		std::filesystem::path projectilePath = getDataPath() / "projectiles" / (fileName + ".json");
		return projectilePath.string();
	}

	std::string getWeaponJsonPath(const std::string fileName)
	{
		std::filesystem::path weaponPath = getDataPath() / "weapons" / (fileName + ".json");
		return weaponPath.string();
	}

	std::string getArmorJsonPath(const std::string fileName, const std::string actorName, const std::string slotName)
	{
		std::filesystem::path equipmentPath = getDataPath() / "armor" / actorName / slotName / (fileName + ".json");
		return equipmentPath.string();
	}

	std::string getGeneralDataJsonPath(const std::string fileName)
	{
		std::filesystem::path generalPath = getDataPath() / "general" / (fileName + ".json");
		return generalPath.string();
	}

	std::string getAssetDataJsonPath(const std::string fileName)
	{
		std::filesystem::path assetDataPath = getDataPath() / "assets" / (fileName + ".json");
		return assetDataPath.string();
	}

	nlohmann::json getJsonData(const std::string filePath)
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

	nlohmann::json getActorJson(const std::string actorName)
	{
		const std::string filePath = getActorJsonPath(actorName);
		return getJsonData(filePath);
	}

	nlohmann::json getProjectileJson(const std::string projectileName)
	{
		const std::string filePath = getProjectileJsonPath(projectileName);
		return getJsonData(filePath);
	}

	nlohmann::json getWeaponJson(const std::string weaponName)
	{
		const std::string filePath = getWeaponJsonPath(weaponName);
		return getJsonData(filePath);
	}

	nlohmann::json getArmorJson(const std::string armorName, const std::string actorName, const std::string slotName)
	{
		const std::string filePath = getArmorJsonPath(armorName, actorName, slotName);
		return getJsonData(filePath);
	}

	nlohmann::json getGeneralDataJson(const std::string dataName)
	{
		const std::string filePath = getGeneralDataJsonPath(dataName);
		return getJsonData(filePath);
	}

	nlohmann::json getAssetDataJson(const std::string assetDataName)
	{
		const std::string filePath = getAssetDataJsonPath(assetDataName);
		return getJsonData(filePath);
	}

	AssetManager::AssetManager(std::shared_ptr<ecs::Manager> manager) : manager(manager), constants(manager->getConstants()) {}

	AssetManager::~AssetManager()
	{
		for (auto& coloredSurface : coloredSurfaces) {
			for (auto& surface : coloredSurface.second)
			{
				SDL_FreeSurface(surface.second);
			}
			coloredSurface.second.clear();
		}
	}

	void AssetManager::loadSurfaces()
	{
		loadMiscSurfaces();
		loadMapSurfaces();
		loadActorSurfaces();
	}

	void AssetManager::loadMiscSurfaces()
	{
		const auto miscSurfacesData = getAssetDataJson("misc")["sprites"];
		const auto path = miscSurfacesData.value("path", "");

		for (const auto& assetGroup : miscSurfacesData["sprite_groups"].items())
		{
			const auto assetGroupPath = path + assetGroup.value().value("path", "");
			for (const auto& textureData : assetGroup.value()["textures"])
			{
				const auto textureName = textureData["name"];
				const auto texturePath = assetGroupPath + std::string(textureData["path"]);

				if (!assetGroup.value().contains("color_variations"))
				{
					addSurface(textureName, texturePath);
					continue;
				}
				loadColorVariationsForSurface(textureName, texturePath, assetGroup.value()["color_variations"]);
			}
		}
	}

	void AssetManager::loadMapSurfaces()
	{
		const auto mapSurfacesData = getAssetDataJson("maps")["spritesheets"];
		const auto path = mapSurfacesData.value("path", "");

		for (const auto& assetGroup : mapSurfacesData["sprite_groups"].items())
		{
			const auto assetGroupPath = path + assetGroup.value().value("path", "");
			for (const auto& textureData : assetGroup.value()["textures"])
			{
				const auto textureName = textureData["name"];
				const auto texturePath = assetGroupPath + std::string(textureData["path"]);

				if (!assetGroup.value().contains("color_variations"))
				{
					addSurface(textureName, texturePath);
					continue;
				}
				loadColorVariationsForSurface(textureName, texturePath, assetGroup.value()["color_variations"]);
			}
		}
	}

	void AssetManager::loadActorSurfaces()
	{
		const auto actorSurfacesData = getAssetDataJson("actors")["spritesheets"];
		const auto path = actorSurfacesData.value("path", "");

		for (const auto& actorAssets : actorSurfacesData["actors"].items())
		{
			const auto actorAssetsPath = path + actorAssets.value().value("path", "");
			const std::vector<std::string> actorWeaponTypes = actorAssets.value()["weapon_types"];

			for (const auto& weaponType : actorWeaponTypes)
			{
				for (const auto& actorColorGroupData : actorAssets.value()["color_groups"].items())
				{
					for (const auto& actorTexture : actorColorGroupData.value()["textures"])
					{
						const auto textureName = weaponType + "_" + std::string(actorTexture["name"]);
						const auto texturePath = actorAssetsPath + weaponType + "/" + std::string(actorTexture["path"]);

						if (!actorColorGroupData.value().contains("color_variations"))
						{
							addSurface(textureName, texturePath);
							continue;
						}
						loadColorVariationsForSurface(textureName, texturePath, actorColorGroupData.value()["color_variations"]);
					}

					if (!actorColorGroupData.value().contains("weapon_textures")) continue;
					for (const auto& weaponTypeEntry : actorColorGroupData.value()["weapon_textures"].items())
					{
						if (weaponTypeEntry.key() == weaponType)
						{
							for (const auto& weaponTexture : weaponTypeEntry.value())
							{
								const auto textureName = weaponType + "_" + std::string(weaponTexture["name"]);
								const auto texturePath = actorAssetsPath + weaponType + "/" + std::string(weaponTexture["path"]);

								if (!actorColorGroupData.value().contains("color_variations"))
								{
									addSurface(textureName, texturePath);
									continue;
								}
								loadColorVariationsForSurface(textureName, texturePath, actorColorGroupData.value()["color_variations"]);
							}
						}
					}
				}
			}
		}
	}

	void AssetManager::loadColorVariationsForSurface(const std::string textureName, const std::string texturePath, nlohmann::json colorVariationsData)
	{
		for (auto& colorVariationData : colorVariationsData.items())
		{
			const auto colorName = colorVariationData.key();
			for (auto& color : colorVariationData.value())
			{
				const auto& srcColorData = color["source_color"];
				SDL_Color srcColor{ srcColorData[0], srcColorData[1], srcColorData[2], srcColorData[3] };
				const auto& targetColorData = color["target_color"];
				SDL_Color targetColor{ targetColorData[0], targetColorData[1], targetColorData[2], targetColorData[3] };
				colorVariations[textureName][colorName].push_back({ srcColor, targetColor });
			}
			addSurface(textureName, texturePath, colorName);
		}
	}

	void AssetManager::loadFonts()
	{
		addFont("arial", "../assets/fonts/arial.ttf", constants->DEBUG_FONT_SIZE);
	}

	void AssetManager::addSurface(const std::string id, const std::string path, const std::string color)
	{
		auto surface = TextureManager::loadSurface(path);
		if (colorVariations.count(id) && colorVariations[id].count(color) && !colorVariations[id][color].empty())
			TextureManager::applyColorMapping(surface, colorVariations[id][color]);
		coloredSurfaces[id][color] = surface;
	}

	SDL_Surface* AssetManager::getSurface(const std::string id, const std::string color) const
	{
		auto it = coloredSurfaces.find(id);
		if (it != coloredSurfaces.end())
		{
			auto coloredSurface = it->second;
			if (coloredSurface.count(color))
				return coloredSurface[color];
			return (*coloredSurface.begin()).second;
		}
		else
		{
			std::cout << "Error: texture not found (" << id << " with color: " << color << ").\n";
			return nullptr;
		}
	}

	void AssetManager::addFont(const std::string id, const std::string path, const int fontSize)
	{
		fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
	}

	TTF_Font* AssetManager::getFont(const std::string id) const
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
}