#include "UILabelComponent.h"
#include "AssetManager.h"
#include "Game.h"

#include <SDL_ttf.h>

ecs::UILabelComponent::UILabelComponent(const int xpos, const int ypos, const std::string& text, const std::string& font, const SDL_Color& color) :
	position({ xpos, ypos, 0, 0 }),
	labelText(text),
	labelFont(font),
	textColor(color),
	labelTexture(nullptr)
{
	SetLabelText(labelText, labelFont);
}

ecs::UILabelComponent::~UILabelComponent()
{
	SDL_DestroyTexture(labelTexture);
}

void ecs::UILabelComponent::init()
{
	setRenderOrder(10);
}

void ecs::UILabelComponent::draw()
{
	SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
}

void ecs::UILabelComponent::SetLabelText(const std::string& text, const std::string_view& font)
{
	if (labelTexture) SDL_DestroyTexture(labelTexture);
	const auto fontId = std::string(font);
	SDL_Surface* surf = TTF_RenderText_Blended(Game::assetManager->getFont(fontId), text.c_str(), textColor);
	labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
	SDL_FreeSurface(surf);
	SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
}
