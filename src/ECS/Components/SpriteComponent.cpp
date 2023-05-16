#include "SpriteComponent.h"
#include "TransformComponent.h"

#include "TextureManager.h"
#include "AssetManager.h"
#include "Animation.h"

ecs::SpriteComponent::SpriteComponent(const std::string_view& surfaceId) :
	surfaceId(surfaceId),
	texture(nullptr),
	srcRect(), destRect()
{
	addSprite("body", std::make_shared<Sprite>(surfaceId, 0));
}

ecs::SpriteComponent::SpriteComponent(const std::string_view& surfaceId, int width, int height) :
	surfaceId(surfaceId),
	texture(nullptr),
	srcRect(), destRect(),
	frameWidth(width),
	frameHeight(height)
{
	addSprite("body", std::make_shared<Sprite>(surfaceId, 0));
}

ecs::SpriteComponent::SpriteComponent(const nlohmann::json& spritesData, const bool isAnimated) :
	texture(nullptr),
	srcRect(), destRect(),
	frameWidth(spritesData["frame_width"]), frameHeight(spritesData["frame_height"]),
	animated(isAnimated), animStartTime(0)
{
	if (spritesData.contains("sprites"))
	{
		for (const auto& spriteData : spritesData["sprites"])
		{
			addSprite
			(
				spriteData["slot"],
				std::make_shared<Sprite>(spriteData["texture"], spriteData.value("z", 0))
			);
		}
	}

	if (spritesData.contains("block_slots"))
	{
		for (const auto& blockedSlotData : spritesData["block_slots"])
		{
			std::vector<std::string> blockedSlotsToAdd;
			for (const auto& blockedSlotName : blockedSlotData["blocked_slots"])
			{
				blockedSlotsToAdd.push_back(blockedSlotName);
			}
			addBlockedSlots(blockedSlotData["blocker_slot"], blockedSlotsToAdd);
		}
	}

	if (animated)
	{
		addAnimationsFromJson(spritesData["animations"]);
		play("idle");
	}
}

ecs::SpriteComponent::~SpriteComponent()
{
	SDL_DestroyTexture(texture);
}

void ecs::SpriteComponent::init()
{
	setRenderOrder(0);
	animStartTime = 0;
	transform = entity->getComponent<ecs::TransformComponent>();

	srcRect.w = frameWidth;
	srcRect.h = frameHeight;
}

void ecs::SpriteComponent::update(double delta)
{
	if (animated && numOfFrames)
	{
		const Uint32 ticks = SDL_GetTicks();
		const Uint32 elapsed = ticks - animStartTime;
		int frameNum = static_cast<int>(elapsed) / animSpeed;
		// check if anim ended
		if (frameNum >= numOfFrames || frameNum < startFrame)
		{
			animStartTime = ticks;
			frameNum = startFrame;
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

		srcRect.x = srcRect.w * frameNum;
		spriteFlip = transform->getDirection().x > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	}

	srcRect.y = (animIndex)*frameHeight;
	destRect.x = static_cast<int>(transform->getPosition().x - static_cast<float>(frameWidth) * transform->getScale() / 2.f);
	destRect.y = static_cast<int>(transform->getPosition().y - static_cast<float>(frameHeight) * transform->getScale() / 2.f);
	destRect.w = static_cast<int>(frameWidth * transform->getScale());
	destRect.h = static_cast<int>(frameHeight * transform->getScale());
}

void ecs::SpriteComponent::draw()
{
	TextureManager::draw(texture, srcRect, destRect, spriteFlip);
}

void ecs::SpriteComponent::addSprite(const std::string& slotName, std::shared_ptr<Sprite> sprite)
{
	sprites[slotName].emplace_back(sprite);
	sortSpritesByZ();
}

void ecs::SpriteComponent::addSpritesToSlot(const std::string& slotName, std::vector<std::shared_ptr<Sprite>> spritesToAdd)
{
	sprites[slotName] = spritesToAdd;
	sortSpritesByZ();
}

void ecs::SpriteComponent::addBlockedSlot(const std::string& blockerName, const std::string& blockedSlotName)
{
	blockedSlots[blockerName].push_back(blockedSlotName);
	sortSpritesByZ();
}

void ecs::SpriteComponent::addBlockedSlots(const std::string& blockerName, const std::vector<std::string>& blockedSlotNames)
{
	for (const auto& slot : blockedSlotNames)
		blockedSlots[blockerName].push_back(slot);
	sortSpritesByZ();
}

void ecs::SpriteComponent::removeSpritesFromSlot(const std::string& slotName)
{
	sprites.erase(slotName);
	blockedSlots.erase(slotName);
	sortSpritesByZ();
}

void ecs::SpriteComponent::removeAllSprites()
{
	sprites.clear();
	sortedSprites.clear();
}

void ecs::SpriteComponent::sortSpritesByZ()
{
	sortedSprites.clear();
	for (auto& [slot, spriteVec] : sprites)
	{
		// check if slot is blocked
		bool isBlocked = false;
		for (auto& [blockerSlotName, blockedSlotNames] : blockedSlots)
		{
			if (std::find(blockedSlotNames.begin(), blockedSlotNames.end(), slot) != blockedSlotNames.end())
			{
				isBlocked = true;
				break;
			}
		}
		if (isBlocked)
			continue;

		for (auto& sprite : spriteVec)
		{
			sortedSprites.emplace_back(sprite);
		}
	}
	std::sort(sortedSprites.begin(), sortedSprites.end(), [](auto& a, auto& b) { return a->getZ() < b->getZ(); });

	SDL_DestroyTexture(texture);
	texture = TextureManager::getCombinedTexture(sortedSprites);
	notify("update_textures");
}

void ecs::SpriteComponent::play(const std::string& newAnimPlay)
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
	startFrame = animData.getStartFrame();
	animIndex = animData.getAnimIndex();
	animSpeed = animData.getAnimSpeed();
	triggerFrames = animData.getTriggerFrames();

	animState = eAnimState::NONE;
	triggered = false;
	incAnimState();
}

void ecs::SpriteComponent::addAnimationsFromJson(const nlohmann::json& animData)
{
	for (auto& [name, animData] : animData.items())
	{
		const int startFrame = animData.value("start_frame", 0);
		const int animIndex = animData.value("anim_index", 0);
		const int num_of_frames = animData.value("num_of_frames", 0);
		const int animSpeed = animData.value("anim_speed", 0);
		const auto trigger_frames = animData.count("trigger_frames") ? animData.at("trigger_frames").get<std::vector<int>>() : std::vector<int>();
		const auto action_name = animData.value("action_name", "");
		addAnimation(name, animIndex, num_of_frames, animSpeed, trigger_frames, action_name, startFrame);
	}
}

void ecs::SpriteComponent::addAnimation(
	const std::string_view& name,
	const int index, const int numOfFrames, const int speed,
	const std::span<const int>& triggerFrames,
	const std::string& actionName,
	const int startFrame)
{
	animations.emplace(name, Animation(index, numOfFrames, speed, triggerFrames, actionName, startFrame));
}

void ecs::SpriteComponent::incAnimState()
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

void ecs::SpriteComponent::sendSignal(const std::string& eventName)
{
	const auto action = actionName.empty() ? animName : actionName;
	notify(action + "_" + eventName);
}