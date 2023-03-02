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
	SpriteComponent(const std::string_view& textureId)
	{
		setTexture(textureId);
	}
	SpriteComponent(const std::string_view& textureId, int width, int height) : frameWidth(width), frameHeight(height)
	{
		setTexture(textureId);
	}
	SpriteComponent(const nlohmann::json& spriteData, const bool isAnimated) : animated(isAnimated)
	{
		frameWidth = spriteData["frame_width"];
		frameHeight = spriteData["frame_height"];
		setTexture(spriteData["texture"]);

		if (animated)
		{
			addAnimationsFromJson(spriteData["animations"]);
			play("idle");
		}
	}

	~SpriteComponent() {}

	void setTexture(const std::string_view& textureId)
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
			// check if anim ended
			if (frameNum >= frames && frameNum % frames == 0)
			{
				animStartTime = ticks;
				frameNum = 0;
				incAnimState();
			}
			// check for trigger events
			if (!triggerFrames.empty())
			{
				auto isTriggerFrame = std::find(triggerFrames.begin(), triggerFrames.end(), frameNum) != triggerFrames.end();
				if (!triggered && isTriggerFrame)
				{
					triggered = true;
					sendSignal("action_start");
				}
				else if (triggered && !isTriggerFrame)
				{
					triggered = false;
					sendSignal("action_stop");
				}
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

	void play(const std::string& newAnimPlay)
	{
		if (animName == newAnimPlay)
			return;
		
		animName = newAnimPlay;
		animStartTime = SDL_GetTicks();
		auto animData = animations[animName];
		frames = animData.frames;
		animIndex = animData.index;
		speed = animData.speed;

		if (animData.frameWidth) frameWidth = animData.frameWidth;
		if (animData.frameHeight) frameHeight = animData.frameHeight;
		if (!animData.triggerFrames.empty()) triggerFrames = animData.triggerFrames;

		animState = eAnimState::NONE;
		triggered = false;
		incAnimState();
	}

	void addAnimationsFromJson(const nlohmann::json& animData)
	{
		for (auto& [name, animData] : animData.items())
		{
			const int id = animData.value("id", 0);
			const int num_of_frames = animData.value("num_of_frames", 0);
			const int speed = animData.value("speed", 0);
			const int frame_width = animData.value("frame_width", 0);
			const int frame_height = animData.value("frame_height", 0);
			const auto trigger_frames = animData.count("trigger_frames") ? animData.at("trigger_frames").get<std::vector<int>>() : std::vector<int>();
			addAnimation(name, id, num_of_frames, speed, frame_width, frame_height, trigger_frames);
		}
	}

	void addAnimation(
		const std::string_view& name,
		const int index, const int numOfFrames, const int speed,
		const int frameWidth = 0, const int frameHeight = 0,
		const std::span<const int>& triggerFrames = {})
	{
		animations.emplace(name, Animation(index, numOfFrames, speed, frameWidth, frameHeight, triggerFrames));
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
			sendSignal(stateStr);
	}

	void sendSignal(const std::string& eventName)
	{
		if (animName == "attack")
			std::cout << animName << "_" << eventName << "\n";
		notify(animName + "_" + eventName);
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
	std::vector<int> triggerFrames;
	bool triggered = false;

	int frameWidth;
	int frameHeight;
};