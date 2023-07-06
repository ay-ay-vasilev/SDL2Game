#include "AimComponent.h"
#include "TransformComponent.h"
#include "TextureManager.h"
#include "KeyboardManager.h"
#include "CameraComponent.h"

ecs::AimComponent::AimComponent() :
	texture(nullptr),
	srcRect(), destRect()
{
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

	std::string texturePath = "assets/images/misc/debug_assets/aim_arrow.png";
	texture = TextureManager::loadTexture(texturePath);
	srcRect = { 0, 0, 48, 48 };

	center =
	{
		static_cast<float>(transform->getPosition().x),
		static_cast<float>(transform->getPosition().y)
	};

	destRect = {
		static_cast<int>(center.x - srcRect.w * transform->getScale() / 2),
		static_cast<int>(center.y - srcRect.h * transform->getScale() / 2 + 10.f), // TODO: load aim offset data from JSON
		static_cast<int>(srcRect.w * transform->getScale()),
		static_cast<int>(srcRect.h * transform->getScale())};
}

void ecs::AimComponent::update(double delta)
{
	calculateRotation(Game::keyboardManager->getMousePos());

	center =
	{
		static_cast<float>(transform->getPosition().x),
		static_cast<float>(transform->getPosition().y)
	};


	destRect = {
		static_cast<int>(center.x - srcRect.w * transform->getScale() / 2),
		static_cast<int>(center.y - srcRect.h * transform->getScale() / 2 + 10.f), // TODO: load aim offset data from JSON
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