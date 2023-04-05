#pragma once
#include "TransformComponent.h"

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
		addSprite("body", std::make_shared<Sprite>(textureId, 0, 0, 0));
	}
	SpriteComponent(const std::string_view& textureId, int width, int height)
	{
		addSprite("body", std::make_shared<Sprite>(textureId, width, height, 0));
	}
	SpriteComponent(const nlohmann::json& spritesData, const bool isAnimated) : animated(isAnimated), animStartTime(0)
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
					spriteData["slot"],
					std::make_shared<Sprite>
					(spriteData["texture"],
					spriteFrameWidth,
					spriteFrameHeight,
					spriteData.value("z", 0))
				);
			}
		}

		if (animated)
		{
			addAnimationsFromJson(spritesData["animations"]);
			for (const auto& sprite : sortedSprites)
			{
				sprite->setAnimNameIndexes(spritesData["animations"]);
			}
			play("idle");
		}
	}

	~SpriteComponent()
	{
	}

	void addSprite(const std::string& slotName, std::shared_ptr<Sprite> sprite)
	{
		sprites[slotName].emplace_back(sprite);
		sortSpritesByZ();
	}

	void addSpritesToSlot(const std::string& slotName, std::vector<std::shared_ptr<Sprite>> spritesToAdd)
	{
		sprites[slotName] = spritesToAdd;
		sortSpritesByZ();
	}

	void removeSpritesFromSlot(const std::string& slotName)
	{
		sprites.erase(slotName);
		sortSpritesByZ();
	}

	void sortSpritesByZ()
	{
		sortedSprites.clear();
		for (auto& [slot, spriteVec] : sprites)
		{
			for (auto& sprite : spriteVec)
			{
				sortedSprites.emplace_back(sprite);
			}
		}
		std::sort(sortedSprites.begin(), sortedSprites.end(), [](auto& a, auto& b) { return a->getZ() < b->getZ(); });
	}

	void init() override
	{
		animStartTime = 0;
		transform = entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		if (animated && numOfFrames)
		{
			const Uint32 ticks = SDL_GetTicks();
			const Uint32 elapsed = ticks - animStartTime;
			int frameNum = static_cast<int>(elapsed) / animSpeed;
			// check if anim ended
			if (frameNum >= numOfFrames && frameNum % numOfFrames == 0)
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
				sprite->setSrcRectXForFrame(static_cast<int>(frameNum % numOfFrames));
			}
			spriteFlip = transform->getDirection().x > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		}

		for (auto& sprite : sortedSprites)
		{
			sprite->setSrcRectYForAnim(animName, animIndex);
			sprite->setDestRect(transform->getPosition(), Vector2D(Game::camera.x, Game::camera.y), transform->getScale());
		}
	}

	void draw() override
	{
		for (const auto& sprite : sortedSprites)
			TextureManager::draw(sprite->getTexture(), sprite->getSrcRect(), sprite->getDestRect(), spriteFlip);
	}

	void play(const std::string& newAnimPlay)
	{
		if (animName == newAnimPlay)
			return;
		if (animations.count(newAnimPlay) == 0)
			return;

		animName = newAnimPlay;
		animStartTime = SDL_GetTicks();
		const auto animData = animations[animName];
		actionName = animData.getActionName();
		numOfFrames = animData.getNumOfFrames();
		animIndex = animData.getAnimIndex();
		animSpeed = animData.getAnimSpeed();
		triggerFrames = animData.getTriggerFrames();

		animState = eAnimState::NONE;
		triggered = false;
		incAnimState();
	}

	void addAnimationsFromJson(const nlohmann::json& animData)
	{
		for (auto& [name, animData] : animData.items())
		{
			const int animIndex = animData.value("anim_index", 0);
			const int num_of_frames = animData.value("num_of_frames", 0);
			const int animSpeed = animData.value("anim_speed", 0);
			const auto trigger_frames = animData.count("trigger_frames") ? animData.at("trigger_frames").get<std::vector<int>>() : std::vector<int>();
			const auto action_name = animData.value("action_name", "");
			addAnimation(name, animIndex, num_of_frames, animSpeed, trigger_frames, action_name);
		}
	}

	void addAnimation(
		const std::string_view& name,
		const int index, const int numOfFrames, const int speed,
		const std::span<const int>& triggerFrames = {},
		const std::string& actionName = "")
	{
		animations.emplace(name, Animation(index, numOfFrames, speed, triggerFrames, actionName));
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
		const auto action = actionName.empty() ? animName : actionName;
		notify(action + "_" + eventName);
	}

private:
	Uint32 animStartTime;
	int animIndex = 0;

	std::map<std::string, Animation> animations;

	std::unordered_map<std::string, std::vector<std::shared_ptr<Sprite>>> sprites;
	std::vector<std::shared_ptr<Sprite>> sortedSprites;

	eAnimState animState = eAnimState::NONE;
	SDL_RendererFlip spriteFlip = SDL_FLIP_HORIZONTAL;

	std::shared_ptr<TransformComponent> transform;

	std::string animName;
	std::string actionName;
	bool animated = false;
	int numOfFrames = 0;
	int animSpeed = 100;
	std::vector<int> triggerFrames;
	bool triggered = false;
};