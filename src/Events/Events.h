#pragma once
#include "Vector2D.h"

namespace events
{
	struct SplatterEvent
	{
		Vector2D splatterPosition{ 0, 0 };
		float intensity{ 0.f };
		std::pair<float, float> radiusRange{ 0, 0 };
		std::pair<float, float> offsetRange{ 0, 0 };
		std::pair<int, int> redRange{ 0, 0 };
		std::pair<int, int> greenRange{ 0, 0 };
		std::pair<int, int> blueRange{ 0, 0 };
		std::pair<int, int> alphaRange{ 0, 0 };
	};

	struct ProjectileEvent
	{
		int ownerId{ 0 };
		Vector2D position{ 0, 0 };
		Vector2D velocity{ 0, 0 };
		std::string filename;
	};
}