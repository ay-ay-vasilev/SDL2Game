#pragma once
#include "ECS.h"
#include "Vector2D.h"

namespace ecs
{
	class TransformComponent;
	class ProjectileComponent : public Component
	{
	public:
		ProjectileComponent(Vector2D velocity, float range);
		
		// Component
		void init() override;
		void update() override;

	private:
		std::shared_ptr<TransformComponent> transform;
		Vector2D velocity{ 0.f, 0.f };
		float range = 0.f;
		float speed = 0.f;
		float distance = 0.f;
	};
}