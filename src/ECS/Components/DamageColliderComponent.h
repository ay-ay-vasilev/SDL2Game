#pragma once
#include "ECS.h"
#include "ColliderShape.h"

namespace ecs
{
	class TransformComponent;
	class DamageColliderComponent : public DrawableComponent
	{
	public:
		explicit DamageColliderComponent(const bool isProjectile);
		virtual ~DamageColliderComponent();

		void init() override;
		void update(double delta) override;
		void draw() override;

		void loadParams(const std::string& name, const bool isProjectile);

		void inline addAffectedTarget(int id) { affectedTargets.emplace_back(id); }
		bool inline isInAffectedTargets(int id) const { return  std::find(affectedTargets.begin(), affectedTargets.end(), id) != affectedTargets.end(); }
		void inline clearAffectedTargets() { affectedTargets.clear(); }
		bool inline isInRange(const std::shared_ptr<ColliderShape>& targetHitbox) const { return damageCollider->collidesWith(targetHitbox); }

		std::shared_ptr<ColliderShape> inline getCollider() const { return damageCollider; }

		bool inline isEnabled() const { return enabled; }
		void inline setEnabled(bool value) { enabled = value; }
		bool inline  isDestroyedOnHit() const { return destroyOnHit; }
		void inline setEnableDraw(bool value) { enableDraw = value; }

	private:
		std::shared_ptr<ecs::TransformComponent> transform;

		std::shared_ptr<ColliderShape> damageCollider;
		Vector2D damageColliderDirectionCoefficient;
		Vector2D damageColliderOffset;

		std::vector<int> affectedTargets;

		SDL_Rect srcRect, destRect;

		SDL_Texture* texture;

		bool enableDraw{ false };
		bool enabled;
		bool destroyOnHit;
	};
}