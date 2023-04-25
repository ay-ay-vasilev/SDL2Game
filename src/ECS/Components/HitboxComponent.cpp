#include "HitboxComponent.h"
#include "TransformComponent.h"
#include "TextureManager.h"
#include "ColliderShape.h"

HitboxComponent::HitboxComponent(const std::string& tag) :
	texture(nullptr),
	srcRect(), destRect(),
	hitbox(), hitboxOffset() {};

HitboxComponent::HitboxComponent(const std::string& tag, const Vector2D& position, const float radius, const Vector2D& hitboxOffset) :
	texture(nullptr),
	srcRect(), destRect(),
	hitbox(std::make_shared<CircleCollider>(position, radius)), hitboxOffset(hitboxOffset) {};

HitboxComponent::HitboxComponent(const std::string& tag, const Vector2D& position, const float width, const float height, const Vector2D& hitboxOffset) :
	texture(nullptr),
	srcRect(), destRect(),
	hitbox(std::make_shared<RectangleCollider>(position, width, height)), hitboxOffset(hitboxOffset) {};

HitboxComponent::HitboxComponent(const std::string& tag, const std::shared_ptr<ColliderShape>& hitboxShape, const Vector2D& hitboxOffset) :
	texture(nullptr),
	srcRect(), destRect(),
	hitbox(hitboxShape), hitboxOffset(hitboxOffset) {};

HitboxComponent::HitboxComponent(const std::string& tag, const nlohmann::json& colliderData) :
	texture(nullptr),
	srcRect(), destRect(),
	hitboxOffset({ colliderData["dx"], colliderData["dy"] })
{
	if (colliderData["shape"] == "circle")
		hitbox = std::make_shared<CircleCollider>(Vector2D(0, 0), colliderData["radius"]);
	if (colliderData["shape"] == "rectangle")
		hitbox = std::make_shared<RectangleCollider>(Vector2D(0, 0), colliderData["w"], colliderData["h"]);
}

HitboxComponent::~HitboxComponent()
{
	SDL_DestroyTexture(texture);
}

void HitboxComponent::init()
{
	setRenderOrder(5);
	if (entity->hasComponent<TransformComponent>())
	{
		transform = entity->getComponent<TransformComponent>();
		hitboxOffset.x *= transform->getScale();
		hitboxOffset.y *= transform->getScale();

		hitbox->setScale(transform->getScale());
		const auto hitboxDX = transform->getPosition().x + hitboxOffset.x;
		const auto hitboxDY = transform->getPosition().y + hitboxOffset.y;
		hitbox->movePosition(Vector2D(hitboxDX, hitboxDY));
	}

	destRect = hitbox->getDrawRect();

	std::string texturePath;
	if (auto rectCollider = std::dynamic_pointer_cast<RectangleCollider>(hitbox)) {
		texturePath = "assets/images/misc/hitbox_rect.png";
	}
	else if (auto circleCollider = std::dynamic_pointer_cast<CircleCollider>(hitbox)) {
		texturePath = "assets/images/misc/hitbox_circle.png";
	}
	texture = TextureManager::loadTexture(texturePath);
	srcRect = { 0, 0, 32, 32 };
}

void HitboxComponent::update()
{
	const auto x = transform->getPosition().x + hitboxOffset.x;
	const auto y = transform->getPosition().y + hitboxOffset.y;
	hitbox->setPosition(Vector2D(x, y));

	destRect = hitbox->getDrawRect();
	destRect.x -= Game::camera.x;
	destRect.y -= Game::camera.y;
}

void HitboxComponent::draw()
{
	if (enableDraw) TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
}