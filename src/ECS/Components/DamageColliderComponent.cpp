#include "DamageColliderComponent.h"
#include "TransformComponent.h"
#include "TextureManager.h"
#include "AssetManager.h"

ecs::DamageColliderComponent::DamageColliderComponent(const bool isProjectile) :
	texture(nullptr),
	enabled(isProjectile),
	destroyOnHit(isProjectile)
{
}

ecs::DamageColliderComponent::~DamageColliderComponent()
{
	SDL_DestroyTexture(texture);
}

void ecs::DamageColliderComponent::init()
{
	setRenderOrder(5);
	if (entity->hasComponent<ecs::TransformComponent>())
	{
		transform = entity->getComponent<ecs::TransformComponent>();
		damageColliderDirectionCoefficient.x *= transform->getScale();
		damageColliderDirectionCoefficient.y *= transform->getScale();
		damageColliderOffset.x *= transform->getScale();
		damageColliderOffset.y *= transform->getScale();

		damageCollider->setScale(transform->getScale());
		const auto weaponColliderX = transform->getPosition().x + damageColliderOffset.x + transform->getDirection().x * damageColliderDirectionCoefficient.x;
		const auto weaponColliderY = transform->getPosition().y + damageColliderOffset.y + transform->getDirection().y * damageColliderDirectionCoefficient.y;
		damageCollider->setPosition(Vector2D(weaponColliderX, weaponColliderY));
	}
	destRect = damageCollider->getDrawRect();
	
	std::string texturePath;
	if (auto rectCollider = std::dynamic_pointer_cast<RectangleCollider>(damageCollider)) {
		texturePath = "assets/images/misc/debug_assets/weapon_collider_rect.png";
	}
	else if (auto circleCollider = std::dynamic_pointer_cast<CircleCollider>(damageCollider)) {
		texturePath = "assets/images/misc/debug_assets/weapon_collider_circle.png";
	}
	texture = TextureManager::loadTexture(texturePath);
	srcRect = { 0, 0, 32, 32 };

}

void ecs::DamageColliderComponent::update(double delta)
{
	const auto weaponColliderX = transform->getPosition().x + damageColliderOffset.x + transform->getDirection().x * damageColliderDirectionCoefficient.x;
	const auto weaponColliderY = transform->getPosition().y + damageColliderOffset.y + transform->getDirection().y * damageColliderDirectionCoefficient.y;
	damageCollider->setPosition(Vector2D(weaponColliderX, weaponColliderY));

	destRect = damageCollider->getDrawRect();
}

void ecs::DamageColliderComponent::draw()
{
	if (!enabled)
		return;

	if (enableDraw) TextureManager::draw(texture, srcRect, destRect);
}

void ecs::DamageColliderComponent::loadParams(const std::string& name, const bool isProjectile)
{
	nlohmann::json weaponData;
	if (isProjectile) weaponData = assets::getProjectileJson(name)["weapon"];
	else weaponData = assets::getWeaponJson(name);

	if (weaponData.contains("collider"))
	{
		const auto& damageColliderData = weaponData["collider"];
		if (damageColliderData["shape"] == "circle")
			damageCollider = std::make_shared<CircleCollider>(Vector2D(0, 0), damageColliderData["radius"]);
		if (damageColliderData["shape"] == "rectangle")
			damageCollider = std::make_shared<RectangleCollider>(Vector2D(0, 0), damageColliderData["w"], damageColliderData["h"]);
		if (damageColliderData.contains("offset"))
			damageColliderOffset = { damageColliderData["offset"]["dx"], damageColliderData["offset"]["dy"] };
		damageColliderDirectionCoefficient = { damageColliderData.value("x", 0.f), damageColliderData.value("y", 0.f) };
	}
}