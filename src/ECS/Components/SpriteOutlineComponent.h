#pragma once
#include "TransformComponent.h"
#include "SpriteComponent.h"

#include "Observer.h"

#include <SDL.h>

class SpriteOutlineComponent : public Component, public Observer
{
public:
	SpriteOutlineComponent() = default;
	SpriteOutlineComponent(const nlohmann::json& outlineData)
	{
	}
	~SpriteOutlineComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void setTexture(SDL_Texture* texture) { this->texture = texture; }

	void init() override
	{
		transformComponent = entity->getComponent<TransformComponent>();
		spriteComponent = entity->getComponent<SpriteComponent>();
		registerWithSubject(spriteComponent);
	}

	void update() override
	{
		srcRect = spriteComponent->getSrcRect();
		destRect = spriteComponent->getDestRect();
		spriteFlip = spriteComponent->getSpriteFlip();
	}

	void draw() override
	{
		SDL_Rect tempDestRect = destRect;
		tempDestRect.x = destRect.x + static_cast<int>(transformComponent->getScale());
		TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
		tempDestRect = destRect;
		tempDestRect.x = destRect.x - static_cast<int>(transformComponent->getScale());
		TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
		tempDestRect = destRect;
		tempDestRect.y = destRect.y + static_cast<int>(transformComponent->getScale());
		TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
		tempDestRect = destRect;
		tempDestRect.y = destRect.y - static_cast<int>(transformComponent->getScale());
		TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
	}

	void onNotify(const std::string_view& observedEvent) override
	{
		if (observedEvent == "update_textures")
		{
			setTexture(TextureManager::getCombinedTexture(spriteComponent->getSortedSprites(), SDL_Color(0, 0, 0, 255)));
		}
	}

private:
	SDL_Texture* texture{ nullptr };

	SDL_Rect srcRect, destRect;
	SDL_RendererFlip spriteFlip;
	std::shared_ptr<TransformComponent> transformComponent;
	std::shared_ptr<SpriteComponent> spriteComponent;
};