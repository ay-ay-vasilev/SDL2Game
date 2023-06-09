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
	const auto stringId = std::string(surfaceId);
	sprites.emplace_back(std::make_shared<Sprite>(stringId, 0));
}

ecs::TileComponent::~TileComponent()
{
	SDL_DestroyTexture(texture);
}

void ecs::TileComponent::init()
{
	setRenderOrder(0);
	updateTexture();
}

void ecs::TileComponent::update(double delta)
{
}

void ecs::TileComponent::draw()
{
	TextureManager::draw(texture, srcRect, destRect);
}

void ecs::TileComponent::updateTexture()
{
	SDL_DestroyTexture(texture);
	texture = TextureManager::getCombinedTexture(sprites);
}

void ecs::TileComponent::applySplatter(Vector2D splatterCenter, int splatterRadius)
{
	// Iterate through the sprites in the tile
	for (const auto& sprite : sprites)
	{
		// Get the sprite's surface
		SDL_Surface* surface = sprite->getSurface();

		Vector2D localSplatterCenter = splatterCenter - position;
		localSplatterCenter.x /= Game::manager->getScale();
		localSplatterCenter.y /= Game::manager->getScale();

		TextureManager::applySplatter(surface, srcRect, localSplatterCenter, splatterRadius);
	}
	updateTexture();
}