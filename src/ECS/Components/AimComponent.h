#pragma once
#include "ECS.h"

#include <SDL_render.h>
#include <numbers>

namespace ecs
{
	class TransformComponent;
	class CameraComponent;
	class AimComponent : public DrawableComponent
	{
	public:
		AimComponent();
		explicit AimComponent(const std::optional<nlohmann::json>& aimComponentData = std::nullopt);
		~AimComponent();

		// Component
		void init() override;
		void update(double delta) override;
		void draw() override;

		void calculateRotation(const Vector2D mousePos);
		void faceAimDirection();

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
		void inline setEnabled(const bool enabled) { this->enabled = enabled; }

	private:
		bool enabled{ false };
		Vector2D center;
		double rotation{};
		std::shared_ptr<TransformComponent> transform{};
		std::shared_ptr<CameraComponent> cameraComponent{};

		Vector2D offset{};
		
		std::string texturePath;
		SDL_Texture* texture{ nullptr };
		SDL_Rect srcRect, destRect;
	};
}