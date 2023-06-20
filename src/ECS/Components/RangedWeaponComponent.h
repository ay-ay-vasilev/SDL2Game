#pragma once
#include "ECS.h"
#include "Observer.h"

#include <SDL.h>
#include <optional>

namespace ecs
{
	class RangedWeaponComponent
	{
	public:
		RangedWeaponComponent() = default;
		explicit RangedWeaponComponent(const std::optional<nlohmann::json>& outlineData = std::nullopt);
		~RangedWeaponComponent();
	};
}