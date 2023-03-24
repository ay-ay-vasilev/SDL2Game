#pragma once

#include "Components.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Animation.h"
#include "Sprite.h"
#include "Subject.h"

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

	SpriteComponent() = default;
	SpriteComponent(const std::string_view& textureId)
	{
		addSprite(textureId, 0);
	}
	SpriteComponent(const std::string_view& textureId, int width, int height) : frameWidth(width), frameHeight(height)
	{
		addSprite(textureId, 0);
	}
	SpriteComponent(const nlohmann::json& spriteData, const bool isAnimated) : animated(isAnimated)
	{
		frameWidth = spriteData["frame_width"];
		frameHeight = spriteData["frame_height"];
		addSprite(spriteData["texture"], 0);

		if (animated)
		{
			addAnimationsFromJson(spriteData["animations"]);
			play("idle");
		}
	}

	~SpriteComponent()
	{
	}

	void addSprite(const std::string_view& textureId, int z)
	{
		const auto texture = Game::assets->getTexture(textureId);
		sprites.push_back(Sprite(texture, z));
	}

	void init() override
	{
		animStartTime = 0;
		transform = entity->getComponent<TransformComponent>();

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;
	}

	void update() override
	{
		if (animated)
		{
			const Uint32 ticks = SDL_GetTicks();
			const Uint32 elapsed = ticks - animStartTime;
			int frameNum = static_cast<int>(elapsed) / speed;
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
				const auto isTriggerFrame = std::find(triggerFrames.begin(), triggerFrames.end(), frameNum) != triggerFrames.end();
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
			spriteFlip = transform->getDirection().x > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		}

		srcRect.y = animIndex * frameHeight;

		destRect.x = static_cast<int>(transform->getPosition().x - static_cast<float>(frameWidth) * transform->getScale() / 2) - Game::camera.x;
		destRect.y = static_cast<int>(transform->getPosition().y - static_cast<float>(frameHeight) * transform->getScale() / 2) - Game::camera.y;
		destRect.w = static_cast<int>(frameWidth * transform->getScale());
		destRect.h = static_cast<int>(frameHeight * transform->getScale());
	}

	void draw() override
	{
		std::sort(sprites.begin(), sprites.end(),
			[](const Sprite& a, const Sprite& b)
			{
				return a.z < b.z;
			});

		for (const auto& sprite : sprites)
		{
			TextureManager::draw(sprite.texture, srcRect, destRect, spriteFlip);
		}
	}

	void play(const std::string& newAnimPlay)
	{
		if (animName == newAnimPlay)
			return;
		
		animName = newAnimPlay;
		animStartTime = SDL_GetTicks();
		const auto animData = animations[animName];
		frames = animData.frames;
		animIndex = animData.index;
		speed = animData.speed;

		if (animData.frameWidth) frameWidth = animData.frameWidth;
		if (animData.frameHeight) frameHeight = animData.frameHeight;

		triggerFrames.clear();
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
		sendSignal(stateStr);
	}

	void sendSignal(const std::string& eventName)
	{
		notify(animName + "_" + eventName);
	}

private:
	Uint32 animStartTime;
	int animIndex = 0;
	std::map<std::string, Animation> animations;
	eAnimState animState = eAnimState::NONE;
	SDL_RendererFlip spriteFlip = SDL_FLIP_HORIZONTAL;

	std::shared_ptr<TransformComponent> transform;
	SDL_Rect srcRect, destRect;

	std::vector<Sprite> sprites;

	std::string animName;
	bool animated = false;
	int frames = 0;
	int speed = 100;
	std::vector<int> triggerFrames;
	bool triggered = false;

	int frameWidth;
	int frameHeight;
};