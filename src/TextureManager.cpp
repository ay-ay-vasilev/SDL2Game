#include "TextureManager.h"
#include <stdexcept>

SDL_Texture* TextureManager::LoadTexture(const std::string_view& fileName)
{
	const std::string tempFileName(fileName);
	std::string imagePath = SDL_GetBasePath();
#ifdef _WIN32
	imagePath = "../";
#endif
	imagePath += fileName;

	SDL_Surface* tempSurface = IMG_Load(imagePath.c_str());
	if (tempSurface == nullptr)
	{
		throw std::runtime_error("Failed to load image: " + tempFileName + " Error: " + IMG_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	if (texture == nullptr)
	{
		throw std::runtime_error("Failder to create texture from surface: " + tempFileName + " Error: " + SDL_GetError());
	}
	SDL_FreeSurface(tempSurface);
	return texture;
}

void TextureManager::Draw(SDL_Texture* tex, const SDL_Rect src, const SDL_Rect dest, const SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);
}
