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
		addSprite(textureId, 0, 0, 0);
	}
	SpriteComponent(const std::string_view& textureId, int width, int height)
	{
		addSprite(textureId, width, height, 0);
	}
	SpriteComponent(const nlohmann::json& spritesData, const bool isAnimated) : animated(isAnimated)
	{
		const auto frameWidth = spritesData["frame_width"];
		const auto frameHeight = spritesData["frame_height"];
		
		if (spritesData.contains("sprites"))
		{
			for (const auto& spriteData : spritesData["sprites"])
			{
				const auto spriteFrameWidth = spriteData.value("frame_width", frameWidth);
				const auto spriteFrameHeight = spriteData.value("frame_height", frameHeight);

				addSprite
				(
					spriteData["texture"],
					spriteFrameWidth,
					spriteFrameHeight,
					spriteData.value("z", 0)
				);
			}
		}

		if (animated)
		{
			addAnimationsFromJson(spritesData["animations"]);
			play("idle");
		}
	}

	~SpriteComponent()
	{
	}

	void setSprite(const std::string& slotName, std::shared_ptr<Sprite> sprite)
	{
		sprites[slotName] = sprite;
		sortSpritesByZ();
	}

	void removeSprite(const std::string& slotName)
	{
		sprites.erase(slotName);
		sortSpritesByZ();
	}

	void sortSpritesByZ()
	{
		sortedSprites.clear();
		sortedSprites.reserve(sprites.size());
		for (auto& [name, sprite] : sprites)
		{
			sortedSprites.emplace_back(name, sprite);
		}
		std::sort(sortedSprites.begin(), sortedSprites.end(), [](auto& a, auto& b) { return a.second->z < b.second->z; });
	}

	void init() override
	{
		animStartTime = 0;
		transform = entity->getComponent<TransformComponent>();
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

			for (auto& sprite : sortedSprites)
			{
				sprite.second->srcRect.x = sprite.second->srcRect.w * static_cast<int>(frameNum % frames);
			}
			spriteFlip = transform->getDirection().x > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		}

		for (auto& sprite : sprites)
		{
			sprite.second->srcRect.y = animIndex * sprite.second->frameHeight;

			sprite.second->destRect.x = static_cast<int>(transform->getPosition().x - static_cast<float>(sprite.second->frameWidth) * transform->getScale() / 2) - Game::camera.x;
			sprite.second->destRect.y = static_cast<int>(transform->getPosition().y - static_cast<float>(sprite.second->frameHeight) * transform->getScale() / 2) - Game::camera.y;
			sprite.second->destRect.w = static_cast<int>(sprite.second->frameWidth * transform->getScale());
			sprite.second->destRect.h = static_cast<int>(sprite.second->frameHeight * transform->getScale());
		}
	}

	void draw() override
	{
		for (const auto& sprite : sortedSprites)
			TextureManager::draw(sprite.second->texture, sprite.second->srcRect, sprite.second->destRect, spriteFlip);
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
			const auto trigger_frames = animData.count("trigger_frames") ? animData.at("trigger_frames").get<std::vector<int>>() : std::vector<int>();
			addAnimation(name, id, num_of_frames, speed, trigger_frames);
		}
	}

	void addAnimation(
		const std::string_view& name,
		const int index, const int numOfFrames, const int speed,
		const std::span<const int>& triggerFrames = {})
	{
		animations.emplace(name, Animation(index, numOfFrames, speed, triggerFrames));
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
	std::unordered_map<std::string, std::shared_ptr<Sprite>> sprites;
	std::vector<std::pair<std::string, std::shared_ptr<Sprite>>> sortedSprites;

	eAnimState animState = eAnimState::NONE;
	SDL_RendererFlip spriteFlip = SDL_FLIP_HORIZONTAL;

	std::shared_ptr<TransformComponent> transform;

	std::string animName;
	bool animated = false;
	int frames = 0;
	int speed = 100;
	std::vector<int> triggerFrames;
	bool triggered = false;
};