#pragma once
#include "ECS.h"
#include "Vector2D.h"
#include <SDL_render.h>

namespace ecs
{
	class TileComponent : public DrawableComponent
	{
	public:
		TileComponent() = default;
		TileComponent(const int srcX, const int srcY, const int xpos, const int ypos, const int tileSize, const std::string_view& surfaceId);
		~TileComponent();

		// Component
		void init() override;
		void update() override;
		void draw() override;

	private:
		SDL_Texture* texture;
		SDL_Rect srcRect, destRect;
		Vector2D position{};
	};
}
