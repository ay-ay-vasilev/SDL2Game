#pragma once
#include "ECS.h"
#include "Observer.h"

#include <SDL.h>
#include <optional>

namespace ecs
{
	class SpriteComponent;
	class ActorComponent;
	class ShadowComponent : public DrawableComponent, public Observer
	{
	public:
		ShadowComponent() = default;
		explicit ShadowComponent(const std::optional<nlohmann::json>& shadowData = std::nullopt);
		~ShadowComponent();

		// Component
		void init() override;
		void update(double delta) override;
		void draw() override;
		// Observer
		void onNotify(const std::string_view& observedEvent) override;

	private:
		std::string texturePath;
		SDL_Texture* texture{ nullptr };

		SDL_Rect srcRect{}, destRect{};

		SDL_RendererFlip spriteFlip;
		std::shared_ptr<SpriteComponent> spriteComponent;
		std::shared_ptr<ActorComponent> actorComponent;
	};
}