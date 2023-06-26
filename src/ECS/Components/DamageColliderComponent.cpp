#include "DamageColliderComponent.h"
#include "TransformComponent.h"
#include "TextureManager.h"
#include "AssetManager.h"

ecs::DamageColliderComponent::DamageColliderComponent(const std::string& name, const bool isProjectile) :
	colliderShape(eColliderShape::RECTANGLE),
	srcRect(), destRect(),
	texture(nullptr),
	enabled(isProjectile),
	destroyOnHit(false),
	damage(0)
{
	nlohmann::json data;
	if (isProjectile) data = assets::getProjectileJson(name)["weapon"];
	else data = assets::getWeaponJson(name);

	parseColliderJson(data);
}

ecs::DamageColliderComponent::~DamageColliderComponent()
{
	SDL_DestroyTexture(texture);
}

void ecs::DamageColliderComponent::init()
{
	setRenderOrder(5);
	transform = entity->getComponent<ecs::TransformComponent>();
	loadWeaponParams();
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
	if (!enabled) return;
	if (enableDraw) TextureManager::draw(texture, srcRect, destRect);
}

void ecs::DamageColliderComponent::assignDamageCollider(const std::string& name)
{
	nlohmann::json data = assets::getWeaponJson(name);
	parseColliderJson(data);
	loadWeaponParams();
}

void ecs::DamageColliderComponent::loadWeaponParams()
{
	damageColliderDirectionCoefficient.x *= transform->getScale();
	damageColliderDirectionCoefficient.y *= transform->getScale();
	damageColliderOffset.x *= transform->getScale();
	damageColliderOffset.y *= transform->getScale();

	damageCollider->setScale(transform->getScale());
	const auto weaponColliderX = transform->getPosition().x + damageColliderOffset.x + transform->getDirection().x * damageColliderDirectionCoefficient.x;
	const auto weaponColliderY = transform->getPosition().y + damageColliderOffset.y + transform->getDirection().y * damageColliderDirectionCoefficient.y;
	damageCollider->setPosition(Vector2D(weaponColliderX, weaponColliderY));

	destRect = damageCollider->getDrawRect();

	std::string texturePath;
	if (colliderShape == eColliderShape::RECTANGLE) {
		texturePath = "assets/images/misc/debug_assets/weapon_collider_rect.png";
	}
	else if (colliderShape == eColliderShape::CIRCLE) {
		texturePath = "assets/images/misc/debug_assets/weapon_collider_circle.png";
	}
	texture = TextureManager::loadTexture(texturePath);
	srcRect = { 0, 0, 32, 32 };
}

void ecs::DamageColliderComponent::parseColliderJson(const nlohmann::json& data)
{
	if (data.contains("collider"))
	{
		const auto& damageColliderData = data["collider"];
		if (damageColliderData["shape"] == "circle")
		{
			colliderShape = eColliderShape::CIRCLE;
			damageCollider = std::make_shared<CircleCollider>(Vector2D(0, 0), damageColliderData["radius"]);
		}
		else if (damageColliderData["shape"] == "rectangle")
		{
			colliderShape = eColliderShape::RECTANGLE;
			damageCollider = std::make_shared<RectangleCollider>(Vector2D(0, 0), damageColliderData["w"], damageColliderData["h"]);
		}

		if (damageColliderData.contains("offset"))
			damageColliderOffset = { damageColliderData["offset"]["dx"], damageColliderData["offset"]["dy"] };
		damageColliderDirectionCoefficient = { damageColliderData.value("x", 0.f), damageColliderData.value("y", 0.f) };
	}
	if (data.contains("damage"))
	{
		damage = data.value("damage", 0);
	}
}