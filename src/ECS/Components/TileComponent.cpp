#include "TileComponent.h"
#include "Game.h"
#include "TextureManager.h"
#include "AssetManager.h"

TileComponent::TileComponent(const int srcX, const int srcY, const int xpos, const int ypos, const int tileSize, const std::string_view& surfaceId) :
	texture(nullptr),
	srcRect{ srcX, srcY, tileSize, tileSize },
	destRect{ xpos, ypos, static_cast<int>(tileSize * Game::manager->getScale()), static_cast<int>(tileSize * Game::manager->getScale()) },
	position { static_cast<float>(xpos), static_cast<float>(ypos) }
{
	texture = TextureManager::getTextureFromSurface(Game::assets->getSurface(surfaceId));
}

TileComponent::~TileComponent()
{
	SDL_DestroyTexture(texture);
}

void TileComponent::update()
{
	destRect.x = static_cast<int>(position.x - Game::camera.x);
	destRect.y = static_cast<int>(position.y - Game::camera.y);
}

void TileComponent::draw()
{
	TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
}
