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

		void addSprite(const std::shared_ptr<Sprite>& sprite);
		void updateTexture();

		Vector2D inline getPosition() const { return position; }
		SDL_Rect inline getSrcRect() const { return srcRect; }
		SDL_Rect inline getDestRect() const { return destRect; }
		SDL_Rect inline getSurfaceRect() const { return surfaceRect; }
		int inline getTileSize() const { return srcRect.w; }

	private:
		std::vector<std::shared_ptr<Sprite>> sprites;
		SDL_Texture* texture;
		SDL_Rect srcRect, destRect;
		SDL_Rect surfaceRect;
		Vector2D position{};
	};
}
