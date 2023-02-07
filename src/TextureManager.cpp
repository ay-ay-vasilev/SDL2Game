#include "TextureManager.h"
#include <stdexcept>

SDL_Texture* TextureManager::LoadTexture(const std::string& fileName)
{
	std::string imagePath = SDL_GetBasePath();
#ifdef _WIN32
	imagePath = "../";
#endif
	imagePath += fileName;

	SDL_Surface* tempSurface = IMG_Load(imagePath.c_str());
	if (tempSurface == nullptr)
	{
		throw std::runtime_error("Failed to load image: " + fileName + " Error: " + IMG_GetError());
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	if (tex == nullptr)
	{
		throw std::runtime_error("Failder to create texture from surface: " + fileName + " Error" + SDL_GetError());
	}
	SDL_FreeSurface(tempSurface);
	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);
}
