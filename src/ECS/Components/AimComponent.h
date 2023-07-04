#pragma once
#include "ECS.h"

#include <SDL_render.h>
#include <numbers>

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

		const Vector2D inline getCenter() const { return center; }
		const Vector2D inline getVelocity() const
		{
			// Calculate the angle in radians
			const double angleRad = rotation * (std::numbers::pi / 180.0);

			// Calculate the vector components
			const double x = std::cos(angleRad);
			const double y = std::sin(angleRad);

			// Create and return the normalized vector
			Vector2D normalizedVector(x, y);
			normalizedVector.Normalize();
			return normalizedVector;
		}

	private:
		Vector2D center;
		double rotation{};
		std::shared_ptr<TransformComponent> transform{};

		SDL_Texture* texture;
		SDL_Rect srcRect, destRect;
	};
}