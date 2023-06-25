#pragma once
#include "ECS.h"
#include "ColliderShape.h"

namespace ecs
{
	class TransformComponent;
	class DamageColliderComponent : public DrawableComponent
	{
	public:
		DamageColliderComponent(const std::string& name = "unarmed", const bool isProjectile = false);
		virtual ~DamageColliderComponent();

		enum class eColliderShape
		{
			RECTANGLE,
			CIRCLE
		};

		void init() override;
		void update(double delta) override;
		void draw() override;

		void loadWeaponParams(const std::string& name);

		void inline addAffectedTarget(int id) { affectedTargets.emplace_back(id); }
		bool inline isInAffectedTargets(int id) const { return  std::find(affectedTargets.begin(), affectedTargets.end(), id) != affectedTargets.end(); }
		void inline clearAffectedTargets() { affectedTargets.clear(); }
		bool inline isInRange(const std::shared_ptr<ColliderShape>& targetHitbox) const { return damageCollider->collidesWith(targetHitbox); }

		std::shared_ptr<ColliderShape> inline getCollider() const { return damageCollider; }

		bool inline isEnabled() const { return enabled; }
		void inline setEnabled(bool value) { enabled = value; }
		bool inline  isDestroyedOnHit() const { return destroyOnHit; }
		void inline setEnableDraw(bool value) { enableDraw = value; }
		int inline getDamage() const { return damage; }

	private:
		void parseColliderJson(const nlohmann::json& data);

		eColliderShape colliderShape;

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

		int damage;
	};
}