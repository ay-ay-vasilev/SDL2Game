#pragma once
#include "TransformComponent.h"
#include "SpriteComponent.h"

#include "Observer.h"

#include <SDL.h>
#include <optional>

class SpriteOutlineComponent : public Component, public Observer
{
public:
	SpriteOutlineComponent() = default;
	SpriteOutlineComponent(const std::optional<nlohmann::json>& outlineData = std::nullopt)
	{
		if (!outlineData)
			return;

		if (outlineData->contains("positions"))
		{
			const auto outlinePositionsData = outlineData.value()["positions"];
			for (const auto& outlinePositionData : outlinePositionsData)
				outlinePositions.emplace_back(Vector2D(outlinePositionData["x"], outlinePositionData["y"]));
		}

		if (outlineData->contains("color"))
		{
			const auto outlineColorData = outlineData.value()["color"];
			outlineColor = SDL_Color(outlineColorData["r"], outlineColorData["g"], outlineColorData["b"], outlineColorData["a"]);
		}
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
		const auto scale = transformComponent->getScale();
		for (const auto& outlinePosition : outlinePositions)
		{
			tempDestRect = SDL_Rect(destRect.x + outlinePosition.x * scale, destRect.y + outlinePosition.y * scale, destRect.w, destRect.h);
			TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
		}
	}

	void onNotify(const std::string_view& observedEvent) override
	{
		if (observedEvent == "update_textures")
		{
			setTexture(TextureManager::getCombinedTexture(spriteComponent->getSortedSprites(), outlineColor));
		}
	}

private:
	SDL_Texture* texture{ nullptr };

	SDL_Rect srcRect, destRect;
	SDL_RendererFlip spriteFlip;
	std::shared_ptr<TransformComponent> transformComponent;
	std::shared_ptr<SpriteComponent> spriteComponent;

	std::vector<Vector2D> outlinePositions;
	SDL_Color outlineColor{0, 0, 0, 0};
};