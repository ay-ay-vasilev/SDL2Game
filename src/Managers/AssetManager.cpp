#include "AssetManager.h"
#include "Constants.h"

AssetManager::AssetManager(std::shared_ptr<Manager> manager, std::shared_ptr<Constants> constants) : manager(manager), constants(constants) {}

AssetManager::~AssetManager() {}

void AssetManager::LoadTextures()
{
	AddTexture("terrain", "assets/images/sprite_sheets/tiles_v0.png");
	AddTexture("player", "assets/images/sprite_sheets/goblin_downscale_spritesheet.png");
	AddTexture("projectile", "assets/images/test_projectile.png");
	AddTexture("enemy", "assets/images/sprite_sheets/human_downscale_spritesheet.png");
}

void AssetManager::LoadFonts()
{
	AddFont("arial", "../assets/fonts/arial.ttf", constants->DEBUG_FONT_SIZE);
}

void AssetManager::AddTexture(const std::string_view id, const std::string_view path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(const std::string_view id) const
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

void AssetManager::AddFont(const std::string_view id, const std::string_view path, const int fontSize)
{
	const std::string tempPath(path);
	fonts.emplace(id, TTF_OpenFont(tempPath.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(const std::string_view id) const
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