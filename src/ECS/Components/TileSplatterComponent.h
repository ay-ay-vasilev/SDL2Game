#pragma once
#include "ECS.h"
#include "Vector2D.h"
#include <SDL_render.h>

class Sprite;
class Splatter;
namespace ecs
{
	class TileComponent;
	class TileSplatterComponent : public DrawableComponent
	{
	public:
		TileSplatterComponent() = default;
		~TileSplatterComponent();

		// Component
		void init() override;
		void update(double delta) override;
		void draw() override;

		void applySplatter(Splatter& splatterData);
	private:
		std::shared_ptr<TileComponent> tileComponent;
		SDL_Surface* splatterSurface;
		SDL_Texture* splatterTexture;

		SDL_Rect srcRect;
		SDL_Rect destRect;
	};
}
