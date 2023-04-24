#include "SpriteComponent.h"
#include "TransformComponent.h"

#include "TextureManager.h"
#include "AssetManager.h"
#include "Animation.h"

SpriteComponent::SpriteComponent(const std::string_view& surfaceId) :
	surfaceId(surfaceId),
	texture(nullptr),
	srcRect(), destRect() {}

SpriteComponent::SpriteComponent(const std::string_view& surfaceId, int width, int height) :
	surfaceId(surfaceId),
	texture(nullptr),
	srcRect(), destRect(),
	frameWidth(width),
	frameHeight(height) {}

SpriteComponent::SpriteComponent(const nlohmann::json& spritesData, const bool isAnimated) :
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
				std::make_shared<Sprite>
				(spriteData["texture"],
					spriteData.value("z", 0))
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

SpriteComponent::~SpriteComponent()
{
	SDL_DestroyTexture(texture);
}

void SpriteComponent::init()
{
	setRenderOrder(0);
	addSprite("body", std::make_shared<Sprite>(surfaceId, 0));

	animStartTime = 0;
	transform = entity->getComponent<TransformComponent>();

	srcRect.w = frameWidth;
	srcRect.h = frameHeight;
}

void SpriteComponent::update()
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

		srcRect.x = srcRect.w * frameNum;
		spriteFlip = transform->getDirection().x > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	}

	srcRect.y = (animIndex)*frameHeight;
	const auto cameraPosition = Vector2D(static_cast<float>(Game::camera.x), static_cast<float>(Game::camera.y));
	destRect.x = static_cast<int>(transform->getPosition().x - static_cast<float>(frameWidth) * transform->getScale() / 2.f - cameraPosition.x);
	destRect.y = static_cast<int>(transform->getPosition().y - static_cast<float>(frameHeight) * transform->getScale() / 2.f - cameraPosition.y);
	destRect.w = static_cast<int>(frameWidth * transform->getScale());
	destRect.h = static_cast<int>(frameHeight * transform->getScale());
}

void SpriteComponent::draw()
{
	TextureManager::draw(texture, srcRect, destRect, spriteFlip);
}

void SpriteComponent::addSprite(const std::string& slotName, std::shared_ptr<Sprite> sprite)
{
	sprites[slotName].emplace_back(sprite);
	sortSpritesByZ();
}

void SpriteComponent::addSpritesToSlot(const std::string& slotName, std::vector<std::shared_ptr<Sprite>> spritesToAdd)
{
	sprites[slotName] = spritesToAdd;
	sortSpritesByZ();
}

void SpriteComponent::addBlockedSlot(const std::string& blockerName, const std::string& blockedSlotName)
{
	blockedSlots[blockerName].push_back(blockedSlotName);
	sortSpritesByZ();
}

void SpriteComponent::addBlockedSlots(const std::string& blockerName, const std::vector<std::string>& blockedSlotNames)
{
	for (const auto& slot : blockedSlotNames)
		blockedSlots[blockerName].push_back(slot);
	sortSpritesByZ();
}

void SpriteComponent::removeSpritesFromSlot(const std::string& slotName)
{
	sprites.erase(slotName);
	blockedSlots.erase(slotName);
	sortSpritesByZ();
}

void SpriteComponent::sortSpritesByZ()
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

	texture = TextureManager::getCombinedTexture(sortedSprites);
	notify("update_textures");
}

void SpriteComponent::play(const std::string& newAnimPlay)
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

void SpriteComponent::addAnimationsFromJson(const nlohmann::json& animData)
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

void SpriteComponent::addAnimation(
	const std::string_view& name,
	const int index, const int numOfFrames, const int speed,
	const std::span<const int>& triggerFrames,
	const std::string& actionName)
{
	animations.emplace(name, Animation(index, numOfFrames, speed, triggerFrames, actionName));
}

void SpriteComponent::incAnimState()
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

void SpriteComponent::sendSignal(const std::string& eventName)
{
	const auto action = actionName.empty() ? animName : actionName;
	notify(action + "_" + eventName);
}