#include "WeaponComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"

WeaponComponent::WeaponComponent(const std::string& name, const std::string& ownerName, bool isProjectile) :
	tag(name),
	ownerTag(ownerName),
	texture(nullptr),
	srcRect(), destRect(),
	weaponColliderDirectionCoefficient({ 0, 0 }),
	weaponColliderOffset({ 0, 0 }),
	enabled(isProjectile),
	destroyOnHit(isProjectile),
	damage(0)
{
	const auto weaponName = std::string(name);
	nlohmann::json weaponData;
	if (isProjectile)
	{
		weaponData = Game::assets->getProjectileJson(weaponName)["weapon"];
	}
	else
	{
		weaponData = Game::assets->getWeaponJson(weaponName);
	}

	damage = weaponData.value("damage", 0);
	weaponType = weaponData.value("weapon_type", "unarmed");

	if (weaponData.contains("collider"))
	{
		const auto& weaponColliderData = weaponData["collider"];
		if (weaponColliderData["shape"] == "circle")
		{
			weaponCollider = std::make_shared<CircleCollider>(Vector2D(0, 0), weaponColliderData["radius"]);
		}
		if (weaponColliderData["shape"] == "rectangle")
		{
			weaponCollider = std::make_shared<RectangleCollider>(Vector2D(0, 0), weaponColliderData["w"], weaponColliderData["h"]);
		}
		if (weaponColliderData.contains("offset")) {
			weaponColliderOffset = { weaponColliderData["offset"]["dx"], weaponColliderData["offset"]["dy"] };
		}
		weaponColliderDirectionCoefficient = { weaponColliderData.value("x", 0.f), weaponColliderData.value("y", 0.f) };
	}

	if (weaponData.contains("sprite_data"))
	{
		if (weaponData["sprite_data"].contains(ownerTag))
		{
			const auto ownerWeaponData = weaponData["sprite_data"][ownerTag];
			const auto weaponSpriteData = ownerWeaponData["sprites"];

			for (const auto data : weaponSpriteData)
			{
				const auto tempZ = data.value("z", 0);
				tempSprites[data["slot"]].emplace_back((std::make_shared<Sprite>(data["texture"], tempZ)));
			}
		}
	}
}

WeaponComponent::~WeaponComponent()
{
	SDL_DestroyTexture(texture);
}

void WeaponComponent::init()
{
	if (entity->hasComponent<TransformComponent>())
	{
		transform = entity->getComponent<TransformComponent>();
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

	spriteComponent = entity->getComponent<SpriteComponent>();
	registerWithSubject(spriteComponent);
	if (tempSprites.empty())
		spriteComponent->removeSpritesFromSlot("weapon");
	for (auto& [slotName, tempSpriteVec] : tempSprites)
	{
		spriteComponent->removeSpritesFromSlot(slotName);
		for (auto& tempSprite : tempSpriteVec)
		{
			spriteComponent->addSprite(slotName, tempSprite);
		}
	}
	tempSprites.clear();
}

void WeaponComponent::update()
{
	const auto weaponColliderX = transform->getPosition().x + weaponColliderOffset.x + transform->getDirection().x * weaponColliderDirectionCoefficient.x;
	const auto weaponColliderY = transform->getPosition().y + weaponColliderOffset.y + transform->getDirection().y * weaponColliderDirectionCoefficient.y;
	weaponCollider->setPosition(Vector2D(weaponColliderX, weaponColliderY));

	destRect = weaponCollider->getDrawRect();
	destRect.x -= Game::camera.x;
	destRect.y -= Game::camera.y;
}

void WeaponComponent::draw()
{
	if (!enabled)
		return;

	TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
}

void WeaponComponent::onNotify(const std::string_view& observedEvent)
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