#pragma once
#include "ECS.h"

#include <SDL_render.h>
#include <wrappedJson.h>
#include "Vector2D.h"

class ColliderShape;
class TextureManager;

namespace ecs
{
	class TransformComponent;
	class HitboxComponent : public DrawableComponent
	{
	public:
		explicit HitboxComponent(const std::string& tag);
		HitboxComponent(const std::string& tag, const Vector2D& position, const float radius, const Vector2D& hitboxOffset = Vector2D());
		HitboxComponent(const std::string& tag, const Vector2D& position, const float width, const float height, const Vector2D& hitboxOffset = Vector2D());
		HitboxComponent(const std::string& tag, const std::shared_ptr<ColliderShape>& hitboxShape, const Vector2D& hitboxOffset = Vector2D());
		HitboxComponent(const std::string& tag, const nlohmann::json& colliderData);
		~HitboxComponent();

		// Component
		void init() override;
		void update(double delta) override;
		void draw() override;

		std::shared_ptr<ColliderShape> inline getHitbox() const { return hitbox; }

		void inline setEnableDraw(bool value) { enableDraw = value; }
	private:
		std::shared_ptr<TransformComponent> transform{nullptr};
		SDL_Texture* texture;
		SDL_Rect srcRect, destRect;
		std::shared_ptr<ColliderShape> hitbox;
		Vector2D hitboxOffset;

		bool enableDraw{ false };
	};
}