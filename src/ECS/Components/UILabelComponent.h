#pragma once
#include "ECS.h"
#include <SDL.h>
#include <string>

class UILabelComponent : public DrawableComponent
{
public:
	UILabelComponent(const int xpos, const int ypos, const std::string& text, const std::string& font, const SDL_Color& color);

	// Component
	void init() override;
	void draw() override;

	void SetLabelText(const std::string& text, const std::string_view& font);

private:
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;
};