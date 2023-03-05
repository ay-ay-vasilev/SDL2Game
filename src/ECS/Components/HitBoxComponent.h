#pragma once
#include "Components.h"
#include "ColliderShape.h"

class HitboxComponent : public Component
{
public:
	HitboxComponent(const std::string_view& tag) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitbox(),
		hitboxOffset() {};

	HitboxComponent(const std::string_view& tag, const Vector2D& position, const float radius, const Vector2D& hitboxOffset = Vector2D()) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitbox(std::make_shared<CircleCollider>(position, radius)),
		hitboxOffset(hitboxOffset) {};

	HitboxComponent(const std::string_view& tag, const Vector2D& position, const int width, const int height, const Vector2D& hitboxOffset = Vector2D()) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitbox(std::make_shared<RectangleCollider>(position, width, height)),
		hitboxOffset(hitboxOffset) {};

	HitboxComponent(const std::string_view& tag, const std::shared_ptr<ColliderShape>& hitboxShape, const Vector2D& hitboxOffset = Vector2D()) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitbox(hitboxShape),
		hitboxOffset(hitboxOffset) {};

	HitboxComponent(const std::string_view& tag, const nlohmann::json& colliderData) :
		tag(tag),
		transform(nullptr), texture(nullptr),
		srcRect(), destRect(),
		hitboxOffset({ colliderData["dx"], colliderData["dy"] })
	{
		if (colliderData["shape"] == "circle")
		{
			hitbox = std::make_shared<CircleCollider>(Vector2D(0, 0), colliderData["radius"]);
		}
		if (colliderData["shape"] == "rectangle")
		{
			hitbox = std::make_shared<RectangleCollider>(Vector2D(0, 0), colliderData["w"], colliderData["h"]);
		}
	}

	void init() override
	{
		id = entity->getID();
		if (entity->hasComponent<TransformComponent>())
		{
			transform = &entity->getComponent<TransformComponent>();
			hitboxOffset.x *= transform->getScale();
			hitboxOffset.y *= transform->getScale();

			hitbox->setScale(transform->getScale());
			const auto hitboxDX = transform->getPosition().x + hitboxOffset.x - (hitbox->getWidth()) / 2;
			const auto hitboxDY = transform->getPosition().y + hitboxOffset.y - (hitbox->getHeight()) / 2;
			hitbox->movePosition(Vector2D(hitboxDX, hitboxDY));
		}

		destRect = hitbox->getDrawRect();

		std::string texturePath;
		if (auto rectCollider = std::dynamic_pointer_cast<RectangleCollider>(hitbox)) {
			texturePath = "assets/images/hitbox_rect.png";
		}
		else if (auto circleCollider = std::dynamic_pointer_cast<CircleCollider>(hitbox)) {
			texturePath = "assets/images/hitbox_circle.png";
		}
		texture = TextureManager::loadTexture(texturePath);
		srcRect = { 0, 0, 32, 32 };
	}

	void update() override
	{
		const auto x = static_cast<int>(transform->getPosition().x + hitboxOffset.x) - (hitbox->getWidth()) / 2;
		const auto y = static_cast<int>(transform->getPosition().y + hitboxOffset.y) - (hitbox->getHeight()) / 2;
		hitbox->setPosition(Vector2D(x, y));

		destRect = hitbox->getDrawRect();
		destRect.x -= Game::camera.x;
		destRect.y -= Game::camera.y;
	}

	void draw() override
	{
		if (debugDraw)
			TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	std::shared_ptr<ColliderShape> getHitbox() const { return hitbox; }

	std::string getTag() const { return tag; }
	int getId() const { return id; }

	void setDebugDraw(bool value) { debugDraw = value; }

private:
	TransformComponent* transform;
	SDL_Texture* texture;

	Vector2D hitboxOffset;
	SDL_Rect srcRect, destRect;

	std::shared_ptr<ColliderShape> hitbox;
	std::string tag;
	bool debugDraw = false;

	int id = 0;
};