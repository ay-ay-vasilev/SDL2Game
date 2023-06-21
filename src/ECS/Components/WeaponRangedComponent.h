#pragma once
#include "ECS.h"
#include "Observer.h"

#include <SDL.h>
#include <optional>

namespace ecs
{
	class WeaponRangedComponent
	{
	public:
		WeaponRangedComponent() = default;
		explicit WeaponRangedComponent(const std::optional<nlohmann::json>& outlineData = std::nullopt);
		~WeaponRangedComponent();
	};
}