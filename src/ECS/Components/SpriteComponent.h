#pragma once
#include "ECS.h"
#include "Subject.h"
#include "Sprite.h"

#include <SDL_render.h>
#include <wrappedJson.h>
#include <map>

class TextureManager;
class AssetManager;
class Animation;

namespace ecs
{
	class TransformComponent;
	class SpriteComponent : public DrawableComponent, public Subject
	{
	public:
		enum class eAnimState : int
		{
			NONE = 0,
			START,
			END
		};

		SpriteComponent() = default;
		explicit SpriteComponent(const std::string_view& surfaceId);
		SpriteComponent(const std::string_view& surfaceId, int width, int height);
		SpriteComponent(const nlohmann::json& spritesData, const bool isAnimated);
		~SpriteComponent() override;

		// Component
		void init() override;
		void update(double delta) override;
		void draw() override;

		void addSprite(const std::string& slotName, std::shared_ptr<Sprite> sprite);
		void addSpritesToSlot(const std::string& slotName, std::vector<std::shared_ptr<Sprite>> spritesToAdd);
		void addBlockedSlot(const std::string& blockerName, const std::string& blockedSlotName);
		void addBlockedSlots(const std::string& blockerName, const std::vector<std::string>& blockedSlotNames);
		void removeSpritesFromSlot(const std::string& slotName);
		void sortSpritesByZ();
		void play(const std::string& newAnimPlay);
		void addAnimationsFromJson(const nlohmann::json& animData);
		void addAnimation(
			const std::string_view& name,
			const int index, const int numOfFrames, const int speed,
			const std::span<const int>& triggerFrames = {},
			const std::string& actionName = "");
		void incAnimState();
		void sendSignal(const std::string& eventName);

		const auto inline getSrcRect() const { return srcRect; }
		const auto inline getDestRect() const { return destRect; }
		const auto inline getSpriteFlip() const { return spriteFlip; }
		const auto inline getSortedSprites() const { return sortedSprites; }

	private:
		Uint32 animStartTime = 0;
		int animIndex = 0;
		std::string surfaceId = "";

		std::map<std::string, Animation> animations;

		std::unordered_map<std::string, std::vector<std::shared_ptr<Sprite>>> sprites; // *key* - slot name, *value* - vector of sprites in a slot
		std::unordered_map<std::string, std::vector<std::string>> blockedSlots; // *key* - blocker slot name, *value* - vector of blocked slots
		std::vector<std::shared_ptr<Sprite>> sortedSprites;

		eAnimState animState = eAnimState::NONE;
		SDL_RendererFlip spriteFlip = SDL_FLIP_HORIZONTAL;

		SDL_Texture* texture;
		std::shared_ptr<TransformComponent> transform;

		SDL_Rect srcRect, destRect;
		const int frameWidth = 0;
		const int frameHeight = 0;
		bool animated = false;
		std::string animName;
		std::string actionName;
		int numOfFrames = 0;
		int animSpeed = 100;
		std::vector<int> triggerFrames;
		bool triggered = false;
	};
}
