#pragma once

#include "Components.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Animation.h"
#include <map>
#include <SDL.h>

class SpriteComponent : public Component
{
public:
	int animIndex = 0;
	std::map<std::string_view, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(const std::string_view textureId)
	{
		setTexture(textureId);
	}
	SpriteComponent(const std::string_view path, const bool isAnimated) : animated(isAnimated)
	{
		Animation idle = Animation(0, 4, 100);
		Animation walk = Animation(1, 8, 80);

		animations.emplace("idle", idle);
		animations.emplace("walk", walk);

		Play("idle");
		setTexture(path);
	}

	~SpriteComponent() {}

	void setTexture(const std::string_view textureId)
	{
		texture = Game::assets->GetTexture(textureId);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = static_cast<int>(transform->width * transform->scale);
		destRect.h = static_cast<int>(transform->height * transform->scale);
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(const std::string_view animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}

private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;
};