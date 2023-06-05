#include "SpriteOutlineComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

#include "TextureManager.h"

ecs::SpriteOutlineComponent::SpriteOutlineComponent(const std::optional<nlohmann::json>& outlineData)
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

ecs::SpriteOutlineComponent::~SpriteOutlineComponent()
{
	SDL_DestroyTexture(texture);
}

void ecs::SpriteOutlineComponent::init()
{
	setRenderOrder(-1);
	transformComponent = entity->getComponent<ecs::TransformComponent>();
	spriteComponent = entity->getComponent<ecs::SpriteComponent>();
	registerWithSubject(spriteComponent);
	enabled = true;
}

void ecs::SpriteOutlineComponent::update(double delta)
{
	if (!enabled) return;

	srcRect = spriteComponent->getSrcRect();
	destRect = spriteComponent->getDestRect();
	spriteFlip = spriteComponent->getSpriteFlip();
}

void ecs::SpriteOutlineComponent::draw()
{
	if (!enabled) return;

	SDL_Rect tempDestRect = destRect;
	const auto scale = transformComponent->getScale();
	for (const auto& outlinePosition : outlinePositions)
	{
		tempDestRect = SDL_Rect(
			destRect.x + static_cast<int>(outlinePosition.x * scale),
			destRect.y + static_cast<int>(outlinePosition.y * scale),
			destRect.w, destRect.h);
		TextureManager::draw(texture, srcRect, tempDestRect, 0.0, spriteFlip);
	}
}

void ecs::SpriteOutlineComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "update_textures")
	{
		SDL_DestroyTexture(texture);
		texture = TextureManager::getCombinedTexture(spriteComponent->getSortedSprites(), outlineColor);
	}
}
