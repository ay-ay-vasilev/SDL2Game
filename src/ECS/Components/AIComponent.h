#pragma once
#include "ECS.h"

namespace ecs
{
	class AIComponent : public Component
	{
	public:
		AIComponent() {}
		virtual ~AIComponent() {}
		virtual void update(Entity& entity) {}
	};
}