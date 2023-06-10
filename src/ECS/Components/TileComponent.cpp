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
	const auto sprite = std::make_shared<Sprite>(stringId, 0);
	surfaceRect = SDL_Rect(0, 0, sprite->getSurface()->w, sprite->getSurface()->h);
	sprites.emplace_back(sprite);
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

void ecs::TileComponent::addSprite(const std::shared_ptr<Sprite>& sprite)
{
	sprites.emplace_back(sprite);
	updateTexture();
}

void ecs::TileComponent::updateTexture()
{
	SDL_DestroyTexture(texture);
	std::sort(sprites.begin(), sprites.end(), [](auto& a, auto& b) { return a->getZ() < b->getZ(); });
	texture = TextureManager::getCombinedTexture(sprites);
}