#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include <SDL_ttf.h>

class Constants;
class AssetManager
{
public:
	AssetManager(std::shared_ptr<Manager> manager, std::shared_ptr<Constants> constants);
	~AssetManager();

	void loadTextures();
	void loadFonts();

	// Texture management
	void addTexture(const std::string_view id, const std::string_view path);
	SDL_Texture* getTexture(const std::string_view id) const;

	// Font management
	void addFont(const std::string_view id, const std::string_view path, const int fontSize);
	TTF_Font* getFont(const std::string_view id) const;

private:

	std::shared_ptr<Manager> manager;
	std::map<const std::string_view, SDL_Texture*> textures;
	std::map<const std::string_view, TTF_Font*> fonts;
	std::shared_ptr<Constants> constants;
};