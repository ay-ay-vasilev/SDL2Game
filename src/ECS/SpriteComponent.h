#pragma once

#include "Components.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include <SDL.h>

class SpriteComponent : public Component
{
public:
	int animIndex = 0;
	std::map<std::string, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string path)
	{
		setTexture(path);
	}
	SpriteComponent(std::string path, bool isAnimated) : animated(isAnimated)
	{
		Animation idle = Animation(0, 4, 100);
		Animation walk = Animation(1, 8, 100);

		animations.emplace("idle", idle);
		animations.emplace("walk", walk);

		Play("idle");
		setTexture(path);
	}

	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void setTexture(std::string path)
	{
		texture = TextureManager::LoadTexture(path);
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

		destRect.x = static_cast<int>(transform->position.x);
		destRect.y = static_cast<int>(transform->position.y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(std::string animName)
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