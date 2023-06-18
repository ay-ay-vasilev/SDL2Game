#include "TintComponent.h"
#include "SpriteComponent.h"

#include "TextureManager.h"

ecs::TintComponent::TintComponent(const std::optional<nlohmann::json>& tintData) : currentTintName("")
{
	if (!tintData) return;

	for (const auto tintColor : tintData.value().items())
	{
		const std::string tintName = tintColor.key();
		tintColors[tintName] = SDL_Color(tintColor.value()["r"], tintColor.value()["g"], tintColor.value()["b"], tintColor.value()["a"]);
	}
}

ecs::TintComponent::~TintComponent()
{
	SDL_DestroyTexture(texture);
}

void ecs::TintComponent::init()
{
	setRenderOrder(1);
	spriteComponent = entity->getComponent<ecs::SpriteComponent>();
	registerWithSubject(spriteComponent);
	enabled = false;
}

void ecs::TintComponent::update(double delta)
{
	if (!enabled) return;

	srcRect = spriteComponent->getSrcRect();
	destRect = spriteComponent->getDestRect();
	spriteFlip = spriteComponent->getSpriteFlip();

	rotation = spriteComponent->getRotation();
}

void ecs::TintComponent::draw()
{
	if (!enabled) return;
	TextureManager::draw(texture, srcRect, destRect, rotation, spriteFlip);
}

void ecs::TintComponent::onNotify(const std::string_view& observedEvent)
{
	if (!enabled) return;

	if (observedEvent == "update_textures")
	{
		SDL_DestroyTexture(texture);
		texture = TextureManager::getCombinedTexture(spriteComponent->getSortedSprites(), tintColors[currentTintName]);
	}
}

void ecs::TintComponent::enableTint(const std::string tintName)
{
	currentTintName = tintName;
	if (!tintColors.count(currentTintName)) return;

	enabled = true;
	SDL_DestroyTexture(texture);
	texture = TextureManager::getCombinedTexture(spriteComponent->getSortedSprites(), tintColors[currentTintName]);
}

void ecs::TintComponent::disableTint()
{
	enabled = false;
}