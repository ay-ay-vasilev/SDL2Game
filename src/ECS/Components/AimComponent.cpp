#include "AimComponent.h"
#include "TransformComponent.h"
#include "TextureManager.h"
#include "KeyboardManager.h"

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
	setRenderOrder(-1);
	
	transform = entity->getComponent<ecs::TransformComponent>();

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
		static_cast<int>(center.y - srcRect.h * transform->getScale() / 2 + 10.f),
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
		static_cast<int>(center.y - srcRect.h * transform->getScale() / 2 + 10.f),
		static_cast<int>(srcRect.w * transform->getScale()),
		static_cast<int>(srcRect.h * transform->getScale()) };
}

void ecs::AimComponent::draw()
{
	TextureManager::draw(texture, srcRect, destRect, rotation + 90.0);
}

void ecs::AimComponent::calculateRotation(const Vector2D mousePos)
{
	const Vector2D newCenter = { static_cast<float>(Game::constants->SCREEN_WIDTH / 2), static_cast<float>(Game::constants->SCREEN_HEIGHT / 2) };
	rotation = Vector2D::Angle(mousePos - newCenter);
}