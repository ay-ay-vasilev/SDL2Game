#include "TextureManager.h"
#include "CameraManager.h"
#include "Sprite.h"
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

SDL_Texture* TextureManager::getCombinedTexture(const std::vector<std::shared_ptr<Sprite>>& sortedSprites, const std::optional<SDL_Color>& textureColor)
{
	// create a new surface to hold the combined image
	SDL_Surface* combinedSurface = nullptr;
	SDL_Texture* texture = nullptr;
	for (const auto& sprite : sortedSprites)
	{
		// get the surface for this sprite
		SDL_Surface* surface = sprite->getSurface();
		if (surface == nullptr)
		{
			continue;
		}

		// if this is the first surface, create the combined surface to match its size and format
		if (combinedSurface == nullptr)
		{
			combinedSurface = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->format);
			if (combinedSurface == nullptr)
			{
				throw std::runtime_error("Failed to create combined surface: " + std::string(SDL_GetError()));
			}
		}

		// copy the surface onto the combined surface
		if (SDL_BlitSurface(surface, nullptr, combinedSurface, nullptr) != 0)
		{
			throw std::runtime_error("Failed to blit surface: " + std::string(SDL_GetError()));
		}
	}

	// create a texture from the combined surface and render it
	if (combinedSurface != nullptr)
	{
		if (textureColor)
		{
			Uint32* pixels = (Uint32*)combinedSurface->pixels;
			int pixelCount = (combinedSurface->w * combinedSurface->h);
			for (int i = 0; i < pixelCount; i++) {
				Uint32 pixel = pixels[i];
				Uint8 alpha = (pixel >> 24) & 0xFF;
				if (alpha != 0) { // non-transparent pixel
					pixels[i] = SDL_MapRGBA(combinedSurface->format, textureColor->r, textureColor->g, textureColor->b, textureColor->a);
				}
			}
		}
		texture = SDL_CreateTextureFromSurface(Game::renderer, combinedSurface);
	}

	// free the combined surface
	if (combinedSurface != nullptr)
	{
		SDL_FreeSurface(combinedSurface);
	}

	return texture;
}

void TextureManager::draw(SDL_Texture* texture, const SDL_Rect source, SDL_Rect destination, const SDL_RendererFlip flip)
{
	destination.x -= Game::cameraManager->getCameraPosition().x;
	destination.y -= Game::cameraManager->getCameraPosition().y;

	if (Game::cameraManager->isInView(destination))
		SDL_RenderCopyEx(Game::renderer, texture, &source, &destination, 0, NULL, flip);
}
