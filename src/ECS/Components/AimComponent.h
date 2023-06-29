#pragma once
#include "ECS.h"

#include <SDL_render.h>

namespace ecs
{
	class TransformComponent;
	class AimComponent : public DrawableComponent
	{
	public:
		AimComponent();
		~AimComponent();

		// Component
		void init() override;
		void update(double delta) override;
		void draw() override;

		void calculateRotation(const Vector2D mousePos);

	private:
		Vector2D center;
		double rotation{};
		std::shared_ptr<TransformComponent> transform{};

		SDL_Texture* texture;
		SDL_Rect srcRect, destRect;
	};
}