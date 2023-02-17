#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include <SDL_ttf.h>

class AssetManager
{
public:
	AssetManager(Manager* manager);
	~AssetManager();

	// Game objects
	void CreateProjectile(const Vector2D pos, const Vector2D size, const Vector2D velocity, const int range, const int speed, const std::string_view id) const;


	// Texture management
	void AddTexture(const std::string_view id, const std::string_view path);
	SDL_Texture* GetTexture(const std::string_view id) const;

	// Font management
	void AddFont(const std::string_view id, const std::string_view path, const int fontSize);
	TTF_Font* GetFont(const std::string_view id) const;

private:

	Manager* manager;
	std::map<const std::string_view, SDL_Texture*> textures;
	std::map<const std::string_view, TTF_Font*> fonts;
};