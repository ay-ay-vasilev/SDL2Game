#pragma once
#include "ECS.h"
#include "Vector2D.h"
#include "ColliderShape.h"

#include <SDL.h>
#include <wrappedJson.h>

#include <string>

class TextureManager;

namespace ecs
{
	class TransformComponent;
	class ColliderComponent : public DrawableComponent
	{
	public:
		explicit ColliderComponent(const std::string& tag);
		ColliderComponent(const std::string& tag, const Vector2D& position, const float radius, const Vector2D& colliderOffset = Vector2D());
		ColliderComponent(const std::string& tag, const Vector2D& position, const float width, const float height, const Vector2D& colliderOffset = Vector2D());
		ColliderComponent(const std::string& tag, const std::shared_ptr<ColliderShape>& colliderShape, const Vector2D& colliderOffset = Vector2D());
		ColliderComponent(const std::string& tag, const nlohmann::json& colliderData);
		~ColliderComponent() override;
		// Component
		void init() override;
		void update(double delta) override;
		void draw() override;

		const bool inline isEnabled() const { return enabled; }
		std::shared_ptr<ColliderShape> inline getCollider() const { return collider; }
		void inline setColliderPos(float x, float y) { collider->setPosition(Vector2D(x, y)); }
		void inline moveColliderPos(float dx, float dy) { collider->movePosition(Vector2D(dx, dy)); }
		float inline getLowestPoint() const { return collider->getLowestPoint(); }

		void inline setEnableDraw(bool value) { enableDraw = value; }
	private:
		std::string tag;

		std::shared_ptr<ColliderShape> collider;
		Vector2D colliderOffset;

		SDL_Rect srcRect{}, destRect{};
		std::shared_ptr<ecs::TransformComponent> transform{ nullptr };
		SDL_Texture* texture{ nullptr };
		bool enabled{ true };
		bool enableDraw{ false };
	};
}