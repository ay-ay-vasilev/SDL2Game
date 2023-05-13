#include "WeaponComponent.h"
#include "TransformComponent.h"
#include "ActorComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"
#include "AssetManager.h"

ecs::WeaponComponent::WeaponComponent(const std::string& name, const std::string& ownerName, bool isProjectile) :
	tag(name),
	texture(nullptr),
	srcRect(), destRect(),
	weaponColliderDirectionCoefficient({ 0, 0 }),
	weaponColliderOffset({ 0, 0 }),
	enabled(isProjectile),
	destroyOnHit(isProjectile),
	damage(0)
{
	nlohmann::json weaponData;
	if (isProjectile) weaponData = assets::getProjectileJson(name)["weapon"];
	else weaponData = assets::getWeaponJson(name);

	damage = weaponData.value("damage", 0);
	weaponType = weaponData.value("weapon_type", "unarmed");

	if (weaponData.contains("collider"))
	{
		const auto& weaponColliderData = weaponData["collider"];
		if (weaponColliderData["shape"] == "circle")
			weaponCollider = std::make_shared<CircleCollider>(Vector2D(0, 0), weaponColliderData["radius"]);
		if (weaponColliderData["shape"] == "rectangle")
			weaponCollider = std::make_shared<RectangleCollider>(Vector2D(0, 0), weaponColliderData["w"], weaponColliderData["h"]);
		if (weaponColliderData.contains("offset"))
			weaponColliderOffset = { weaponColliderData["offset"]["dx"], weaponColliderData["offset"]["dy"] };
		weaponColliderDirectionCoefficient = { weaponColliderData.value("x", 0.f), weaponColliderData.value("y", 0.f) };
	}

	if (weaponData.contains("sprite_data"))
	{
		if (weaponData["sprite_data"].contains(ownerName))
		{
			const auto ownerWeaponData = weaponData["sprite_data"][ownerName];
			const auto weaponSpriteData = ownerWeaponData["sprites"];

			for (const auto data : weaponSpriteData)
			{
				const auto tempZ = data.value("z", 0);
				tempSprites.push_back({ data["slot"], data["texture"], tempZ });
			}
		}
	}
}

ecs::WeaponComponent::~WeaponComponent()
{
	SDL_DestroyTexture(texture);
}

void ecs::WeaponComponent::init()
{
	setRenderOrder(5);
	if (entity->hasComponent<ecs::TransformComponent>())
	{
		transform = entity->getComponent<ecs::TransformComponent>();
		weaponColliderDirectionCoefficient.x *= transform->getScale();
		weaponColliderDirectionCoefficient.y *= transform->getScale();
		weaponColliderOffset.x *= transform->getScale();
		weaponColliderOffset.y *= transform->getScale();

		weaponCollider->setScale(transform->getScale());
		const auto weaponColliderX = transform->getPosition().x + weaponColliderOffset.x + transform->getDirection().x * weaponColliderDirectionCoefficient.x;
		const auto weaponColliderY = transform->getPosition().y + weaponColliderOffset.y + transform->getDirection().y * weaponColliderDirectionCoefficient.y;
		weaponCollider->setPosition(Vector2D(weaponColliderX, weaponColliderY));
	}
	destRect = weaponCollider->getDrawRect();

	std::string texturePath;
	if (auto rectCollider = std::dynamic_pointer_cast<RectangleCollider>(weaponCollider)) {
		texturePath = "assets/images/misc/weapon_collider_rect.png";
	}
	else if (auto circleCollider = std::dynamic_pointer_cast<CircleCollider>(weaponCollider)) {
		texturePath = "assets/images/misc/weapon_collider_circle.png";
	}
	texture = TextureManager::loadTexture(texturePath);
	srcRect = { 0, 0, 32, 32 };


	actorComponent = entity->getComponent<ecs::ActorComponent>();
	if (actorComponent)
	{
		registerWithSubject(actorComponent);

		actorComponent->setWeaponType(weaponType);
		if (tempSprites.empty()) actorComponent->removeSpritesFromSlot("weapon");
		for (const auto& spriteData : tempSprites)
			actorComponent->removeSpritesFromSlot(spriteData.slotName);
		for (const auto& spriteData : tempSprites)
		{
			actorComponent->addSprite(spriteData.slotName, spriteData.textureName, spriteData.z);
		}
		tempSprites.clear();
	}
	else
	{
		spriteComponent = entity->getComponent<ecs::SpriteComponent>();
		registerWithSubject(spriteComponent);

		if (tempSprites.empty()) spriteComponent->removeSpritesFromSlot("weapon");
		for (const auto& spriteData : tempSprites)
			spriteComponent->removeSpritesFromSlot(spriteData.slotName);
		for (const auto& spriteData : tempSprites)
		{
			spriteComponent->addSprite(spriteData.slotName, std::make_shared<Sprite>(spriteData.textureName, spriteData.z));
		}
		tempSprites.clear();
	}
}

void ecs::WeaponComponent::update(double delta)
{
	const auto weaponColliderX = transform->getPosition().x + weaponColliderOffset.x + transform->getDirection().x * weaponColliderDirectionCoefficient.x;
	const auto weaponColliderY = transform->getPosition().y + weaponColliderOffset.y + transform->getDirection().y * weaponColliderDirectionCoefficient.y;
	weaponCollider->setPosition(Vector2D(weaponColliderX, weaponColliderY));

	destRect = weaponCollider->getDrawRect();
}

void ecs::WeaponComponent::draw()
{
	if (!enabled)
		return;

	if (enableDraw) TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
}

void ecs::WeaponComponent::onNotify(const std::string_view& observedEvent)
{
	if (observedEvent == "attack_action_start")
	{
		enabled = true;
	}
	if (observedEvent == "attack_action_stop")
	{
		enabled = false;
		affectedTargets.clear();
	}
	if (observedEvent == "idle_start" || observedEvent == "walk_start")
	{
		enabled = false;
	}
}