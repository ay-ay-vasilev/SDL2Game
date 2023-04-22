#pragma once
#include "ECS.h"

#include <SDL_render.h>

class ColliderShape;
class TextureManager;
class TransformComponent;
class HitboxComponent : public Component
{
public:
	HitboxComponent(const std::string& tag);
	HitboxComponent(const std::string& tag, const Vector2D& position, const float radius, const Vector2D& hitboxOffset = Vector2D());
	HitboxComponent(const std::string& tag, const Vector2D& position, const float width, const float height, const Vector2D& hitboxOffset = Vector2D());
	HitboxComponent(const std::string& tag, const std::shared_ptr<ColliderShape>& hitboxShape, const Vector2D& hitboxOffset = Vector2D());
	HitboxComponent(const std::string& tag, const nlohmann::json& colliderData);
	~HitboxComponent();

	// Component
	void init() override;
	void update() override;
	void draw() override;

	std::shared_ptr<ColliderShape> inline getHitbox() const { return hitbox; }
	std::string inline getTag() const { return tag; }
	const int inline getId() const { return id; }

private:
	int id = 0;
	std::string tag;

	std::shared_ptr<TransformComponent> transform{nullptr};
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	std::shared_ptr<ColliderShape> hitbox;
	Vector2D hitboxOffset;
};