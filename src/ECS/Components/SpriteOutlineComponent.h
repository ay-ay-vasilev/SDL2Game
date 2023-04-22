#pragma once
#include "ECS.h"
#include "Observer.h"

#include <SDL.h>
#include <optional>

class TransformComponent;
class SpriteComponent;
class SpriteOutlineComponent : public Component, public Observer
{
public:
	SpriteOutlineComponent() = default;
	SpriteOutlineComponent(const std::optional<nlohmann::json>& outlineData = std::nullopt);

	~SpriteOutlineComponent();

	void inline setTexture(SDL_Texture* texture) { this->texture = texture; }

	// Component
	void init() override;
	void update() override;
	void draw() override;
	// Observer
	void onNotify(const std::string_view& observedEvent) override;

private:
	SDL_Texture* texture{ nullptr };

	SDL_Rect srcRect{}, destRect{};

	SDL_RendererFlip spriteFlip;
	std::shared_ptr<TransformComponent> transformComponent;
	std::shared_ptr<SpriteComponent> spriteComponent;

	std::vector<Vector2D> outlinePositions;
	SDL_Color outlineColor{0, 0, 0, 0};
};