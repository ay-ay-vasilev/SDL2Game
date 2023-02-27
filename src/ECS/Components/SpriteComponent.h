#pragma once

#include "Components.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Animation.h"
#include "Subject.h"

#include <SDL.h>
#include <json.hpp>

#include <map>

class SpriteComponent : public Component, public Subject
{
public:
	enum class eAnimState : int
	{
		NONE = 0,
		START,
		END
	};
	SDL_RendererFlip spriteFlip = SDL_FLIP_HORIZONTAL;

	SpriteComponent() = default;
	SpriteComponent(const std::string_view textureId)
	{
		setTexture(textureId);
	}
	SpriteComponent(const std::string_view textureId, int width, int height) : frameWidth(width), frameHeight(height)
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
		animStartTime = 0;
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
			Uint32 ticks = SDL_GetTicks();
			Uint32 elapsed = ticks - animStartTime;
			auto frameNum = elapsed / speed;
			if (frameNum >= frames && frameNum % frames == 0)
			{
				animStartTime = ticks;
				frameNum = 0;
				incAnimState();
			}

			srcRect.x = srcRect.w * static_cast<int>(frameNum % frames);
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

	void play(const std::string_view animNameView)
	{
		std::string animNameStr(animNameView);
		if (animName == animNameStr)
			return;
		
		animName = animNameStr;
		animStartTime = SDL_GetTicks();
		auto animData = animations[animName];
		frames = animData.frames;
		animIndex = animData.index;
		speed = animData.speed;

		if (animData.frameWidth) frameWidth = animData.frameWidth;
		if (animData.frameHeight) frameHeight = animData.frameHeight;

		animState = eAnimState::NONE;
		incAnimState();
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

	void incAnimState()
	{
		if (static_cast<int>(animState) + 1 > static_cast<int>(eAnimState::END))
		{
			animState = eAnimState::NONE;
		}

		animState = static_cast<eAnimState>(static_cast<int>(animState) + 1);

		std::string stateStr;
		switch (animState)
		{
		case eAnimState::START:
			stateStr = "start";
			break;
		case eAnimState::END:
			stateStr = "end";
			break;
		}
		if (animName == "attack")
			notify(animName + "_" + stateStr);
	}

private:
	Uint32 animStartTime;
	int animIndex = 0;
	std::map<std::string, Animation> animations;
	eAnimState animState = eAnimState::NONE;

	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	std::string animName;
	bool animated = false;
	int frames = 0;
	int speed = 100;

	int frameWidth;
	int frameHeight;
};