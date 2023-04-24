#include "ColliderComponent.h"
#include "TextureManager.h"
#include "TransformComponent.h"

ColliderComponent::ColliderComponent(const std::string& tag) :
	tag(tag),
	collider(),
	colliderOffset() {};

ColliderComponent::ColliderComponent(const std::string& tag, const Vector2D& position, const float radius, const Vector2D& colliderOffset) :
	tag(tag),
	collider(std::make_shared<CircleCollider>(position, radius)),
	colliderOffset(colliderOffset) {};

ColliderComponent::ColliderComponent(const std::string& tag, const Vector2D& position, const float width, const float height, const Vector2D& colliderOffset) :
	tag(tag),
	collider(std::make_shared<RectangleCollider>(position, width, height)),
	colliderOffset(colliderOffset) {};

ColliderComponent::ColliderComponent(const std::string& tag, const std::shared_ptr<ColliderShape>& colliderShape, const Vector2D& colliderOffset) :
	tag(tag),
	collider(colliderShape),
	colliderOffset(colliderOffset) {};

ColliderComponent::ColliderComponent(const std::string& tag, const nlohmann::json& colliderData) :
	tag(tag),
	colliderOffset({ colliderData["dx"], colliderData["dy"] })
{
	if (colliderData["shape"] == "circle")
	{
		collider = std::make_shared<CircleCollider>(Vector2D(0, 0), colliderData["radius"]);
	}
	if (colliderData["shape"] == "rectangle")
	{
		collider = std::make_shared<RectangleCollider>(Vector2D(0, 0), colliderData["w"], colliderData["h"]);
	}
}

ColliderComponent::~ColliderComponent()
{
	SDL_DestroyTexture(texture);
}

void ColliderComponent::init()
{
	setRenderOrder(5);
	if (entity->hasComponent<TransformComponent>())
	{
		transform = entity->getComponent<TransformComponent>();
		colliderOffset.x *= transform->getScale();
		colliderOffset.y *= transform->getScale();

		collider->setScale(transform->getScale());
		const auto colliderDX = transform->getPosition().x + colliderOffset.x;
		const auto colliderDY = transform->getPosition().y + colliderOffset.y;
		collider->movePosition(Vector2D(colliderDX, colliderDY));
	}

	destRect = collider->getDrawRect();

	std::string texturePath;
	if (auto rectCollider = std::dynamic_pointer_cast<RectangleCollider>(collider)) {
		texturePath = "assets/images/misc/collider_rect.png";
	}
	else if (auto circleCollider = std::dynamic_pointer_cast<CircleCollider>(collider)) {
		texturePath = "assets/images/misc/collider_circle.png";
	}
	texture = TextureManager::loadTexture(texturePath);

	srcRect = { 0, 0, 32, 32 };
}

void ColliderComponent::update()
{
	if (tag != "terrain")
	{
		const auto x = transform->getPosition().x + colliderOffset.x;
		const auto y = transform->getPosition().y + colliderOffset.y;
		collider->setPosition(Vector2D(x, y));
	}

	destRect = collider->getDrawRect();
	destRect.x -= Game::camera.x;
	destRect.y -= Game::camera.y;
}

void ColliderComponent::draw()
{
	TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
}