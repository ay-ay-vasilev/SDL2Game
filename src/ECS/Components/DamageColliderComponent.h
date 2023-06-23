#pragma once
#include "ECS.h"
#include "ColliderShape.h"

namespace ecs
{
	class TransformComponent;
	class DamageColliderComponent : public Component
	{
	public:
		DamageColliderComponent() {}
		virtual ~DamageColliderComponent() {}

		void init() override;
		void update(double delta) override;

		void inline addAffectedTarget(int id) { affectedTargets.emplace_back(id); }
		bool inline isInAffectedTargets(int id) const { return  std::find(affectedTargets.begin(), affectedTargets.end(), id) != affectedTargets.end(); }
		bool inline isInRange(const std::shared_ptr<ColliderShape>& targetHitbox) const { return damageCollider->collidesWith(targetHitbox); }

		std::shared_ptr<ColliderShape> inline getCollider() const { return damageCollider; }

	private:
		std::shared_ptr<ecs::TransformComponent> transform;

		std::shared_ptr<ColliderShape> damageCollider;
		Vector2D weaponColliderDirectionCoefficient;
		Vector2D weaponColliderOffset;

		std::vector<int> affectedTargets;

		SDL_Rect srcRect, destRect;

	};
}