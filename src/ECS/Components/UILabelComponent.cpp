#include "UILabelComponent.h"
#include "AssetManager.h"
#include "Game.h"

#include <SDL_ttf.h>

UILabelComponent::UILabelComponent(const int xpos, const int ypos, const std::string& text, const std::string& font, const SDL_Color& color) :
	position({ xpos, ypos, 0, 0 }),
	labelText(text),
	labelFont(font),
	textColor(color),
	labelTexture(nullptr)
{
	SetLabelText(labelText, labelFont);
}

void UILabelComponent::draw()
{
	SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	SDL_DestroyTexture(labelTexture);
}

void UILabelComponent::SetLabelText(const std::string& text, const std::string_view& font)
{
	SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->getFont(font), text.c_str(), textColor);
	labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
	SDL_FreeSurface(surf);
	SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
}
