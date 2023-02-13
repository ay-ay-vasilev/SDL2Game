#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"

class AssetManager
{
public:
	AssetManager(Manager* manager);
	~AssetManager();

	// Game objects
	void CreateProjectile(Vector2D pos, Vector2D velocity, int range, int speed, std::string id);


	// Texture management
	void AddTexture(std::string id, std::string path);
	SDL_Texture* GetTexture(std::string id);

private:

	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;

};