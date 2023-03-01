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
	std::string getActorJson(const std::string& fileName);
	std::string getProjectileJson(const std::string& fileName);

	// Texture management
	void addTexture(const std::string_view& id, const std::string& path);
	SDL_Texture* getTexture(const std::string_view& id) const;

	// Font management
	void addFont(const std::string_view& id, const std::string& path, const int fontSize);
	TTF_Font* getFont(const std::string_view& id) const;

private:

	std::shared_ptr<Manager> manager;
	std::map<const std::string_view, SDL_Texture*> textures;
	std::map<const std::string_view, TTF_Font*> fonts;
	std::shared_ptr<Constants> constants;
};