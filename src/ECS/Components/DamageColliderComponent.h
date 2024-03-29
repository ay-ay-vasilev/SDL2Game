#pragma once
#include "ECS.h"
#include "ColliderShape.h"

#include <wrappedJson.h>

namespace ecs
{
	class TransformComponent;
	class DamageColliderComponent : public DrawableComponent
	{
	public:
		DamageColliderComponent(const std::string& name = "unarmed", const bool isProjectile = false);
		~DamageColliderComponent();

		enum class eColliderShape
		{
			RECTANGLE,
			CIRCLE
		};

		void init() override;
		void update(double delta) override;
		void draw() override;

		void assignDamageCollider(const std::string& name);

		bool addAffectedTarget(Uint32 id);
		const bool inline isInAffectedTargets(Uint32 id) const { return  std::find(affectedTargets.begin(), affectedTargets.end(), id) != affectedTargets.end(); }
		void inline clearAffectedTargets() { affectedTargets.clear(); }
		const bool inline isInRange(const std::shared_ptr<ColliderShape>& targetHitbox) const { return damageCollider->collidesWith(targetHitbox); }

		std::shared_ptr<ColliderShape> inline getCollider() const { return damageCollider; }

		const bool inline isEnabled() const { return enabled; }
		void inline setEnabled(bool value) { enabled = value; }
		const bool inline  isDestroyedOnHit() const { return destroyOnHit; }
		void inline setEnableDraw(bool value) { enableDraw = value; }
		const int inline getDamage() const { return damage; }
		const std::string inline getTag() const { return tag; }
		const Vector2D inline getOffset() const { return damageColliderOffset; }
		void inline setOffset(const Vector2D& offset) { damageColliderOffset = offset; }

	private:
		void parseColliderJson(const nlohmann::json& data);
		void loadWeaponParams();

		eColliderShape colliderShape;

		std::shared_ptr<ecs::TransformComponent> transform;

		std::shared_ptr<ColliderShape> damageCollider;
		Vector2D damageColliderDirectionCoefficient;
		Vector2D damageColliderOffset;

		std::vector<Uint32> affectedTargets;
		SDL_Rect srcRect, destRect;
		SDL_Texture* texture;

		std::string tag;

		bool enableDraw{ false };
		bool enabled;
		bool destroyOnHit;
		int maxAffectedTargets;

		int damage;

		constexpr static int UNLIMITED_TARGETS = -1;
	};
}