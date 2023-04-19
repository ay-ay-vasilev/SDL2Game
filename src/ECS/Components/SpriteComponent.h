#pragma once
#include "TransformComponent.h"

#include "TextureManager.h"
#include "AssetManager.h"
#include "Animation.h"
#include "Sprite.h"
#include "Subject.h"

#include <wrappedJson.h>

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
	SpriteComponent(const std::string_view& textureId) : frameWidth(0), frameHeight(0)
	{
		addSprite("body", std::make_shared<Sprite>(textureId, 0));
	}
	SpriteComponent(const std::string_view& textureId, int width, int height) : frameWidth(width), frameHeight(height)
	{
		addSprite("body", std::make_shared<Sprite>(textureId, 0));
	}
	SpriteComponent(const nlohmann::json& spritesData, const bool isAnimated) :
		animated(isAnimated), animStartTime(0),
		frameWidth(spritesData["frame_width"]), frameHeight(spritesData["frame_height"])
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

	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
		SDL_DestroyTexture(blackTexture);
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

	void addBlockedSlot(const std::string& blockerName, const std::string& blockedSlotName)
	{
		blockedSlots[blockerName].push_back(blockedSlotName);
		sortSpritesByZ();
	}

	void addBlockedSlots(const std::string& blockerName, const std::vector<std::string>& blockedSlotNames)
	{
		for (const auto& slot : blockedSlotNames)
			blockedSlots[blockerName].push_back(slot);
		sortSpritesByZ();
	}

	void removeSpritesFromSlot(const std::string& slotName)
	{
		sprites.erase(slotName);
		blockedSlots.erase(slotName);
		sortSpritesByZ();
	}

	void sortSpritesByZ()
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

		createTexture();
		createBlackTexture();
	}

	void createTexture()
	{
		// create a new surface to hold the combined image
		SDL_Surface* combinedSurface = nullptr;
		for (const auto& sprite : sortedSprites)
		{
			// get the surface for this sprite
			SDL_Surface* surface = sprite->getSurface();
			if (surface == nullptr)
			{
				continue;
			}

			// if this is the first surface, create the combined surface to match its size and format
			if (combinedSurface == nullptr)
			{
				combinedSurface = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->format);
				if (combinedSurface == nullptr)
				{
					throw std::runtime_error("Failed to create combined surface: " + std::string(SDL_GetError()));
				}
			}

			// copy the surface onto the combined surface
			if (SDL_BlitSurface(surface, nullptr, combinedSurface, nullptr) != 0)
			{
				throw std::runtime_error("Failed to blit surface: " + std::string(SDL_GetError()));
			}
		}

		// create a texture from the combined surface and render it
		if (combinedSurface != nullptr)
		{
			texture = SDL_CreateTextureFromSurface(Game::renderer, combinedSurface);
		}

		// free the combined surface
		if (combinedSurface != nullptr)
		{
			SDL_FreeSurface(combinedSurface);
		}
	}

	void createBlackTexture()
	{
		// create a new surface to hold the combined image
		SDL_Surface* combinedSurface = nullptr;
		for (const auto& sprite : sortedSprites)
		{
			// get the surface for this sprite
			SDL_Surface* surface = sprite->getSurface();
			if (surface == nullptr)
			{
				continue;
			}

			// if this is the first surface, create the combined surface to match its size and format
			if (combinedSurface == nullptr)
			{
				combinedSurface = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->format);
				if (combinedSurface == nullptr)
				{
					throw std::runtime_error("Failed to create combined surface: " + std::string(SDL_GetError()));
				}
			}

			// copy the surface onto the combined surface
			if (SDL_BlitSurface(surface, nullptr, combinedSurface, nullptr) != 0)
			{
				throw std::runtime_error("Failed to blit surface: " + std::string(SDL_GetError()));
			}
		}

		// create a texture from the combined surface and render it
		if (combinedSurface != nullptr)
		{
			Uint32* pixels = (Uint32*)combinedSurface->pixels;
			int pixelCount = (combinedSurface->w * combinedSurface->h);
			for (int i = 0; i < pixelCount; i++) {
				Uint32 pixel = pixels[i];
				Uint8 alpha = (pixel >> 24) & 0xFF;
				if (alpha != 0) { // non-transparent pixel
					pixels[i] = SDL_MapRGBA(combinedSurface->format, 0, 0, 0, 255);
				}
			}

			blackTexture = SDL_CreateTextureFromSurface(Game::renderer, combinedSurface);
		}

		// free the combined surface
		if (combinedSurface != nullptr)
		{
			SDL_FreeSurface(combinedSurface);
		}
	}

	void init() override
	{
		animStartTime = 0;
		transform = entity->getComponent<TransformComponent>();

		srcRect.w = frameWidth;
		srcRect.h = frameHeight;
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

	void draw() override
	{
		SDL_Rect originalDestRect = destRect;
		SDL_Rect destRect = originalDestRect;
		destRect.x = originalDestRect.x + transform->getScale();
		TextureManager::draw(blackTexture, srcRect, destRect, spriteFlip);
		destRect = originalDestRect;
		destRect.x = originalDestRect.x - transform->getScale();
		TextureManager::draw(blackTexture, srcRect, destRect, spriteFlip);
		destRect = originalDestRect;
		destRect.y = originalDestRect.y + transform->getScale();
		TextureManager::draw(blackTexture, srcRect, destRect, spriteFlip);
		destRect = originalDestRect;
		destRect.y = originalDestRect.y - transform->getScale();
		TextureManager::draw(blackTexture, srcRect, destRect, spriteFlip);

		TextureManager::draw(texture, srcRect, originalDestRect, spriteFlip);
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

	std::unordered_map<std::string, std::vector<std::shared_ptr<Sprite>>> sprites; // *key* - slot name, *value* - vector of sprites in a slot
	std::vector<std::shared_ptr<Sprite>> sortedSprites;
	std::unordered_map<std::string, std::vector<std::string>> blockedSlots; // *key* - blocker slot name, *value* - vector of blocked slots

	eAnimState animState = eAnimState::NONE;
	SDL_RendererFlip spriteFlip = SDL_FLIP_HORIZONTAL;

	SDL_Texture* texture;
	SDL_Texture* blackTexture;
	std::shared_ptr<TransformComponent> transform;

	SDL_Rect srcRect, destRect;
	const int frameWidth;
	const int frameHeight;
	std::string animName;
	std::string actionName;
	bool animated = false;
	int numOfFrames = 0;
	int animSpeed = 100;
	std::vector<int> triggerFrames;
	bool triggered = false;
};