#include "AssetManager.h"
#include "Constants.h"

AssetManager::AssetManager(std::shared_ptr<Manager> manager, std::shared_ptr<Constants> constants) : manager(manager), constants(constants) {}

AssetManager::~AssetManager() {}

void AssetManager::loadTextures()
{
	addTexture("terrain", "assets/images/spritesheets/tiles_v0.png");
	addTexture("goblin", "assets/images/spritesheets/goblin_downscale_spritesheet.png");
	addTexture("projectile", "assets/images/test_projectile.png");
	addTexture("human", "assets/images/spritesheets/human_downscale_spritesheet.png");
}

void AssetManager::loadFonts()
{
	addFont("arial", "../assets/fonts/arial.ttf", constants->DEBUG_FONT_SIZE);
}

void AssetManager::addTexture(const std::string_view id, const std::string_view path)
{
	textures.emplace(id, TextureManager::loadTexture(path));
}

SDL_Texture* AssetManager::getTexture(const std::string_view id) const
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

void AssetManager::addFont(const std::string_view id, const std::string_view path, const int fontSize)
{
	const std::string tempPath(path);
	fonts.emplace(id, TTF_OpenFont(tempPath.c_str(), fontSize));
}

TTF_Font* AssetManager::getFont(const std::string_view id) const
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