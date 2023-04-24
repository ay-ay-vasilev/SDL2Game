#include "SpriteOutlineComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

#include "TextureManager.h"

SpriteOutlineComponent::SpriteOutlineComponent(const std::optional<nlohmann::json>& outlineData)
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

SpriteOutlineComponent::~SpriteOutlineComponent()
{
	SDL_DestroyTexture(texture);
}

void SpriteOutlineComponent::init()
{
	setRenderOrder(-1);
	transformComponent = entity->getComponent<TransformComponent>();
	spriteComponent = entity->getComponent<SpriteComponent>();
	registerWithSubject(spriteComponent);
}

void SpriteOutlineComponent::update()
{
	srcRect = spriteComponent->getSrcRect();
	destRect = spriteComponent->getDestRect();
	spriteFlip = spriteComponent->getSpriteFlip();
}

void SpriteOutlineComponent::draw()
{
	SDL_Rect tempDestRect = destRect;
	const auto scale = transformComponent->getScale();
	for (const auto& outlinePosition : outlinePositions)
	{
		tempDestRect = SDL_Rect(destRect.x + outlinePosition.x * scale, destRect.y + outlinePosition.y * scale, destRect.w, destRect.h);
		TextureManager::draw(texture, srcRect, tempDestRect, spriteFlip);
	}
}

void SpriteOutlineComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "update_textures")
		setTexture(TextureManager::getCombinedTexture(spriteComponent->getSortedSprites(), outlineColor));
}
