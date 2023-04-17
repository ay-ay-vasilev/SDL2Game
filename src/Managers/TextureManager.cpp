#include "TextureManager.h"
#include <stdexcept>

SDL_Texture* TextureManager::loadTexture(const std::string& fileName)
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
	auto texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	if (texture == nullptr)
	{
		throw std::runtime_error("Failed to create texture from surface: " + fileName + " Error: " + SDL_GetError());
	}
	SDL_FreeSurface(tempSurface);
	return texture;
}

SDL_Surface* TextureManager::loadSurface(const std::string& fileName)
{
	std::string imagePath = SDL_GetBasePath();
#ifdef _WIN32
	imagePath = "../";
#endif
	imagePath += fileName;

	SDL_Surface* surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr)
	{
		throw std::runtime_error("Failed to load image: " + fileName + " Error: " + IMG_GetError());
	}
	return surface;
}

SDL_Texture* TextureManager::getTextureFromSurface(SDL_Surface* surface)
{
	auto texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to create texture from surface! Error: ") + SDL_GetError());
	}
	return texture;
}

void TextureManager::draw(SDL_Texture* texture, const SDL_Rect source, const SDL_Rect destination, const SDL_RendererFlip flip)
{
	if (destination.x < (Game::camera.x + Game::camera.w + destination.w) && destination.x >= ((-1) * destination.w))
		if (destination.y < (Game::camera.y + Game::camera.h + destination.h) && destination.y >= ((-1) * destination.h))
			SDL_RenderCopyEx(Game::renderer, texture, &source, &destination, 0, NULL, flip);
}
