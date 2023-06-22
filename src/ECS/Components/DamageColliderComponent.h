#pragma once
#include "ECS.h"

class ColliderShape;

namespace ecs
{
	class DamageColliderComponent : public Component
	{
	public:
		DamageColliderComponent() {}
		virtual ~DamageColliderComponent() {}

		void inline addAffectedTarget(int id) { affectedTargets.emplace_back(id); }
		bool inline isInAffectedTargets(int id) const { return  std::find(affectedTargets.begin(), affectedTargets.end(), id) != affectedTargets.end(); }

		std::shared_ptr<ColliderShape> inline getCollider() const { return damageCollider; }

	private:
		std::shared_ptr<ColliderShape> damageCollider;
		std::vector<int> affectedTargets;
	};
}