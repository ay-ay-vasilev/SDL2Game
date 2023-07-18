#pragma once
#include "ECS.h"

namespace ecs
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent() {}

		void inline setPositionOnScreen(const Vector2D& position)
		{
			positionOnScreen = position;
		}

		const Vector2D inline getPositionOnScreen() { return positionOnScreen; }

	private:
		Vector2D positionOnScreen;
	};
}