#pragma once
#include "ECS.h"
#include "Observer.h"

#include <SDL.h>
#include <optional>

namespace ecs
{
	class SpriteComponent;
	class TintComponent : public DrawableComponent, public Observer
	{
	public:
		TintComponent() = default;
		explicit TintComponent(const std::optional<nlohmann::json>& tintData = std::nullopt);
		~TintComponent();

		// Component
		void init() override;
		void update(double delta) override;
		void draw() override;
		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		void enableTint(const std::string tintName);
		void disableTint();

	private:
		SDL_Texture* texture{ nullptr };

		SDL_Rect srcRect{}, destRect{};

		SDL_RendererFlip spriteFlip;
		std::shared_ptr<SpriteComponent> spriteComponent;

		std::unordered_map<std::string, SDL_Color> tintColors;
		std::string currentTintName;

		bool enabled{ false };
	};
}