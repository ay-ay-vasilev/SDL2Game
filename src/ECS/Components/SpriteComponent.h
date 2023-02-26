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
	SpriteComponent(const nlohmann::json& spriteData, const bool isAnimated) : animated(isAnimated)
	{
		if (animated)
		{
			frameWidth = spriteData["frame_width"];
			frameHeight = spriteData["frame_height"];
			addAnimationsFromJson(spriteData["animations"]);
			play("idle");
		}
		setTexture(spriteData["texture"]);
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
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * frameHeight;

		destRect.x = static_cast<int>(transform->position.x) - (frameWidth * transform->scale / 2) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - (frameHeight * transform->scale / 2) - Game::camera.y;
		destRect.w = static_cast<int>(frameWidth * transform->scale);
		destRect.h = static_cast<int>(frameHeight * transform->scale);
	}

	void draw() override
	{
		TextureManager::draw(texture, srcRect, destRect, spriteFlip);
	}

	void play(const std::string_view animName)
	{
		std::string animNameStr(animName);
		auto animData = animations[animNameStr];

		frames = animData.frames;
		animIndex = animData.index;
		speed = animData.speed;

		if (animData.frameWidth) frameWidth = animData.frameWidth;
		if (animData.frameHeight) frameHeight = animData.frameHeight;
	}

	void addAnimationsFromJson(const nlohmann::json& animData)
	{
		for (auto& [name, animData] : animData.items())
		{
			addAnimation(name, animData["id"], animData["num_of_frames"], animData["speed"],
				animData.value("frame_width", 0), animData.value("frame_height", 0));
		}
	}

	void addAnimation(const std::string_view name, const int index, const int numOfFrames, const int speed, const int frameWidth = 0, const int frameHeight = 0)
	{
		animations.emplace(name, Animation(index, numOfFrames, speed, frameWidth, frameHeight));
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

	int frameWidth;
	int frameHeight;
};