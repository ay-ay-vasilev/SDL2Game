#include "TileComponent.h"
#include "Game.h"
#include "TextureManager.h"
#include "AssetManager.h"

ecs::TileComponent::TileComponent(const int srcX, const int srcY, const int xpos, const int ypos, const int tileSize, const std::string_view& surfaceId) :
	texture(nullptr),
	srcRect{ srcX, srcY, tileSize, tileSize },
	destRect{ xpos, ypos, static_cast<int>(tileSize * Game::manager->getScale()), static_cast<int>(tileSize * Game::manager->getScale()) },
	position { static_cast<float>(xpos), static_cast<float>(ypos) }
{
	texture = TextureManager::getTextureFromSurface(Game::assets->getSurface(surfaceId));
}

ecs::TileComponent::~TileComponent()
{
	SDL_DestroyTexture(texture);
}

void ecs::TileComponent::init()
{
	setRenderOrder(0);
}

void ecs::TileComponent::update(double delta)
{
	destRect.x = static_cast<int>(position.x - Game::camera.x);
	destRect.y = static_cast<int>(position.y - Game::camera.y);
}

void ecs::TileComponent::draw()
{
	TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
}
