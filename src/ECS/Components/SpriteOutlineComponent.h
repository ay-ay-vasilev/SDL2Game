#pragma once
#include "ECS.h"
#include "Observer.h"

#include <SDL.h>
#include <optional>

namespace ecs
{
	class TransformComponent;
	class SpriteComponent;
	class SpriteOutlineComponent : public DrawableComponent, public Observer
	{
	public:
		SpriteOutlineComponent() = default;
		explicit SpriteOutlineComponent(const std::optional<nlohmann::json>& outlineData = std::nullopt);
		~SpriteOutlineComponent();

		// Component
		void init() override;
		void update(double delta) override;
		void draw() override;
		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		void inline setEnabled(const bool enabled) { this->enabled = enabled; }

	private:
		SDL_Texture* texture{ nullptr };

		SDL_Rect srcRect{}, destRect{};
		double rotation{ 0.0 };

		SDL_RendererFlip spriteFlip;
		std::shared_ptr<TransformComponent> transformComponent;
		std::shared_ptr<SpriteComponent> spriteComponent;

		std::vector<Vector2D> outlinePositions;
		SDL_Color outlineColor{0, 0, 0, 0};

		bool enabled{ false };
	};
}

