#pragma once

#include "AssetManager.h"
#include "Game.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILabelComponent : public Component
{
public:
	UILabelComponent(const int xpos, const int ypos, const std::string_view& text, const std::string_view& font, const SDL_Color& color) :
		position({xpos, ypos, 0, 0}),
		labelText(text),
		labelFont(font),
		textColor(color),
		labelTexture(nullptr)
	{
		SetLabelText(labelText, labelFont);
	}
	~UILabelComponent()
	{
	}

	void SetLabelText(const std::string& text, const std::string_view& font)
	{
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->getFont(font), text.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);
		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void draw() override
	{
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
		SDL_DestroyTexture(labelTexture);
	}

private:
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;
};