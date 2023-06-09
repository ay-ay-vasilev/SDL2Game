#pragma once
#include "ECS.h"
#include "Vector2D.h"
#include <SDL_render.h>
#include "Sprite.h"

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
		void update(double delta) override;
		void draw() override;

		void updateTexture();
		void applySplatter(Vector2D splatterCenter, int splatterRadius);

		Vector2D inline getPosition() const { return position; }
		int inline getTileSize() const { return srcRect.w; }

	private:
		std::vector<std::shared_ptr<Sprite>> sprites;
		SDL_Texture* texture;
		SDL_Rect srcRect, destRect;
		Vector2D position{};
	};
}
