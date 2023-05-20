#include "ShadowComponent.h"
#include "SpriteComponent.h"	
#include "ActorComponent.h"

#include "TextureManager.h"
#include "Game.h"

ecs::ShadowComponent::ShadowComponent(const std::optional<nlohmann::json>& shadowData)
{
	if (!shadowData)
		return;

	if (shadowData->contains("texture"))
	{
		texturePath = shadowData.value()["texture"];
	}
}

ecs::ShadowComponent::~ShadowComponent()
{
	SDL_DestroyTexture(texture);
}

void ecs::ShadowComponent::init()
{
	setRenderOrder(-2);
	spriteComponent = entity->getComponent<ecs::SpriteComponent>();
	registerWithSubject(spriteComponent);
	actorComponent = entity->getComponent<ecs::ActorComponent>();

	const auto surface = Game::assetManager->getSurface(actorComponent->getWeaponType() + "_" + texturePath);
	texture = TextureManager::getTextureFromSurface(surface);
}

void ecs::ShadowComponent::update(double delta)
{
	srcRect = spriteComponent->getSrcRect();
	destRect = spriteComponent->getDestRect();
	spriteFlip = spriteComponent->getSpriteFlip();
}

void ecs::ShadowComponent::draw()
{
	SDL_Rect tempDestRect = destRect;
	TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
}

void ecs::ShadowComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "update_textures")
	{
		SDL_DestroyTexture(texture);
		const auto surface = Game::assetManager->getSurface(actorComponent->getWeaponType() + "_" + texturePath);
		texture = TextureManager::getTextureFromSurface(surface);
	}
}
