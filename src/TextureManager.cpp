#include "TextureManager.h"
#include <stdexcept>

SDL_Texture* TextureManager::LoadTexture(const std::string& fileName, SDL_Renderer* ren)
{
    std::string imagePath = SDL_GetBasePath();
    imagePath += fileName;

	SDL_Surface* tempSurface = IMG_Load(imagePath.c_str());
	if (tempSurface == nullptr)
	{
		throw std::runtime_error("Failed to load image: " + fileName + " Error: " + IMG_GetError());
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
	if (tex == nullptr)
	{
		throw std::runtime_error("Failder to create texture from surface: " + fileName + " Error" + SDL_GetError());
	}
	SDL_FreeSurface(tempSurface);
	return tex;
}
