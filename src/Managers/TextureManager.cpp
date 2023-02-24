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

void TextureManager::Draw(SDL_Texture* texture, const SDL_Rect source, const SDL_Rect destination, const SDL_RendererFlip flip)
{
	if (destination.x < (Game::camera.w + destination.w) && destination.x >= ((-1) * destination.w))
		if (destination.y < (Game::camera.h + destination.h) && destination.y >= ((-1) * destination.h))
			SDL_RenderCopyEx(Game::renderer, texture, &source, &destination, 0, NULL, flip);
}
