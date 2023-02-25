#pragma once

#include "Components.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Animation.h"

#include <SDL.h>
#include <json.hpp>

#include <map>

class SpriteComponent : public Component
{
public:

	SDL_RendererFlip spriteFlip = SDL_FLIP_HORIZONTAL;

	SpriteComponent() = default;
	SpriteComponent(const std::string_view textureId)
	{
		setTexture(textureId);
	}
	SpriteComponent(const std::string_view texturePath, const nlohmann::json& animationsData, const bool isAnimated) : animated(isAnimated)
	{
		if (animated)
		{
			addAnimationsFromJson(animationsData);
			play("idle");
		}
		setTexture(texturePath);
	}

	~SpriteComponent() {}

	void setTexture(const std::string_view textureId)
	{
		texture = Game::assets->getTexture(textureId);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = static_cast<int>(transform->width * transform->scale);
		destRect.h = static_cast<int>(transform->height * transform->scale);
	}

	void draw() override
	{
		TextureManager::draw(texture, srcRect, destRect, spriteFlip);
	}

	void play(const std::string_view animName)
	{
		std::string animNameStr(animName);

		frames = animations[animNameStr].frames;
		animIndex = animations[animNameStr].index;
		speed = animations[animNameStr].speed;
	}

	void addAnimationsFromJson(const nlohmann::json& animData)
	{
		for (auto& [name, animData] : animData.items())
		{
			addAnimation(name, animData["id"], animData["num_of_frames"], animData["speed"]);
		}
	}

	void addAnimation(const std::string_view name, const int index, const int numOfFrames, const int speed)
	{
		animations.emplace(name, Animation(index, numOfFrames, speed));
	}

private:
	int animIndex = 0;
	std::map<std::string, Animation> animations;

	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;
};