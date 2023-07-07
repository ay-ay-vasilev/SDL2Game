#include "AimComponent.h"
#include "TransformComponent.h"
#include "TextureManager.h"
#include "KeyboardManager.h"
#include "CameraComponent.h"
#include "AssetManager.h"

ecs::AimComponent::AimComponent() :
	texturePath(),
	texture(nullptr),
	srcRect(), destRect(), offset()
{
}

ecs::AimComponent::AimComponent(const std::optional<nlohmann::json>& aimComponentData)
{
	if (!aimComponentData)
		return;

	if (aimComponentData->contains("texture"))
	{
		texturePath = aimComponentData.value()["texture"];
	}
	offset = { aimComponentData.value().value("dx", 0.f), aimComponentData.value().value("dy", 0.f) };
	srcRect = { 0, 0, aimComponentData.value().value("w", 0), aimComponentData.value().value("h", 0) };
}

ecs::AimComponent::~AimComponent()
{
	SDL_DestroyTexture(texture);
}

void ecs::AimComponent::init()
{
	setRenderOrder(-2);
	
	transform = entity->getComponent<ecs::TransformComponent>();
	cameraComponent = entity->getComponent<ecs::CameraComponent>();

	texture = TextureManager::getTextureFromSurface(Game::assetManager->getSurface(texturePath));

	center =
	{
		static_cast<float>(transform->getPosition().x + offset.x * transform->getScale()),
		static_cast<float>(transform->getPosition().y + offset.y * transform->getScale())
	};

	destRect = {
		static_cast<int>(center.x - srcRect.w * transform->getScale() / 2),
		static_cast<int>(center.y - srcRect.h * transform->getScale() / 2),
		static_cast<int>(srcRect.w * transform->getScale()),
		static_cast<int>(srcRect.h * transform->getScale())};
}

void ecs::AimComponent::update(double delta)
{
	calculateRotation(Game::keyboardManager->getMousePos());

	center =
	{
		static_cast<float>(transform->getPosition().x + offset.x * transform->getScale()),
		static_cast<float>(transform->getPosition().y + offset.y * transform->getScale())
	};


	destRect = {
		static_cast<int>(center.x - srcRect.w * transform->getScale() / 2),
		static_cast<int>(center.y - srcRect.h * transform->getScale() / 2),
		static_cast<int>(srcRect.w * transform->getScale()),
		static_cast<int>(srcRect.h * transform->getScale()) };
}

void ecs::AimComponent::draw()
{
	if (!enabled) return;
	TextureManager::draw(texture, srcRect, destRect, rotation + 90.0);
}

void ecs::AimComponent::calculateRotation(const Vector2D mousePos)
{
	const auto positionOnScreen = cameraComponent->getPositionOnScreen();
	rotation = Vector2D::Angle(mousePos - positionOnScreen);
}

void ecs::AimComponent::faceAimDirection()
{
	if ((rotation < -90 && rotation > -180) || (rotation > 90 && rotation < 180))
		transform->setDirectionX(-1);
	else if ((rotation > -90 && rotation < 90))
		transform->setDirectionX(1);
}