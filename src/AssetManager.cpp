#include "AssetManager.h"
#include "Components.h"

AssetManager::AssetManager(Manager* manager) : manager(manager) {}

AssetManager::~AssetManager() {}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D velocity, int range, int speed, std::string id)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(velocity, range, speed);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::PROJECTILES);
}

void AssetManager::AddTexture(std::string id, std::string path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	return fonts[id];
}

