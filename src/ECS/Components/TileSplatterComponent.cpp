#include "TileSplatterComponent.h"
#include "TileComponent.h"

#include "TextureManager.h"
#include "Sprite.h"
#include "Splatter.h"

ecs::TileSplatterComponent::~TileSplatterComponent()
{
	SDL_FreeSurface(splatterSurface);
	SDL_DestroyTexture(splatterTexture);
}

void ecs::TileSplatterComponent::init()
{
	tileComponent = entity->getComponent<ecs::TileComponent>();

	srcRect = tileComponent->getSrcRect();
	srcRect.x = 0;
	srcRect.y = 0;

	destRect = tileComponent->getDestRect();

	splatterSurface = SDL_CreateRGBSurfaceWithFormat(0, srcRect.w, srcRect.h, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_FillRect(splatterSurface, NULL, SDL_MapRGBA(splatterSurface->format, 0, 0, 0, 0));

	splatterTexture = TextureManager::getTextureFromSurface(splatterSurface);
	setRenderOrder(1);
}

void ecs::TileSplatterComponent::update(double delta)
{
}

void ecs::TileSplatterComponent::draw()
{
	TextureManager::draw(splatterTexture, srcRect, destRect);
}

void ecs::TileSplatterComponent::applySplatter(const Splatter& splatterData)
{
	const auto position = tileComponent->getPosition();

	Vector2D localSplatterCenter = splatterData.getSplatterCenter() - position;
	localSplatterCenter.x /= Game::manager->getScale();
	localSplatterCenter.y /= Game::manager->getScale();

	TextureManager::applySplatter(splatterSurface, localSplatterCenter, tileComponent->getTileSize(), splatterData);

	SDL_DestroyTexture(splatterTexture);
	splatterTexture = TextureManager::getTextureFromSurface(splatterSurface);
}