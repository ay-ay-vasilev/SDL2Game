#include "TextureManager.h"
#include "CameraManager.h"
#include "Sprite.h"
#include "Splatter.h"
#include "Vector2D.h"
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

void TextureManager::applyColorMapping(SDL_Surface* surface, const std::vector<std::pair<SDL_Color, SDL_Color>>& colorMapping)
{
	// Lock the surface to make sure we can access the pixel data
	SDL_LockSurface(surface);

	// Iterate over each pixel in the surface and apply the color mapping
	for (int y = 0; y < surface->h; y++)
	{
		for (int x = 0; x < surface->w; x++)
		{
			// Get the pixel color at (x, y)
			Uint32 pixel = get_pixel(surface, x, y);

			// Extract the RGBA components from the pixel color
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

			if (!a) continue;

			// Create an SDL_Color object for the current pixel color
			SDL_Color currentColor = { r, g, b, a };

			// Check if the current pixel color is in the color mapping
			auto it = std::find_if(colorMapping.begin(), colorMapping.end(), [&](const std::pair<SDL_Color, SDL_Color>& pair)
				{
					return pair.first.r == currentColor.r && pair.first.g == currentColor.g && pair.first.b == currentColor.b && pair.first.a == currentColor.a;
				});

			if (it != colorMapping.end())
			{
				// If it is, replace the pixel color with the corresponding color in the mapping
				SDL_Color mappedColor = it->second;
				pixel = SDL_MapRGBA(surface->format, mappedColor.r, mappedColor.g, mappedColor.b, mappedColor.a);
				set_pixel(surface, x, y, pixel);
			}
		}
	}

	// Unlock the surface
	SDL_UnlockSurface(surface);
}

void TextureManager::applySplatter(SDL_Surface* surface, const Vector2D& position, const int tileSize, const Splatter& splatterData)
{
	Vector2D splatterCenter = position;
	splatterCenter.x = std::floor(splatterCenter.x);
	splatterCenter.y = std::floor(splatterCenter.y);

	int splatterRadius = splatterData.getSplatterRadius();

	int left = std::max(0, static_cast<int>(splatterCenter.x) - splatterRadius);
	int right = std::min(tileSize, static_cast<int>(splatterCenter.x) + splatterRadius);
	int top = std::max(0, static_cast<int>(splatterCenter.y) - splatterRadius);
	int bottom = std::min(tileSize, static_cast<int>(splatterCenter.y) + splatterRadius);

	if (left >= right || top >= bottom) return;


	int minRedValue = splatterData.getRedRange().first;
	int maxRedValue = splatterData.getRedRange().second;
	int minGreenValue = splatterData.getGreenRange().first;
	int maxGreenValue = splatterData.getGreenRange().second;
	int minBlueValue = splatterData.getBlueRange().first;
	int maxBlueValue = splatterData.getBlueRange().second;
	int minAlphaValue = splatterData.getAlphaRange().first;
	int maxAlphaValue = splatterData.getAlphaRange().second;
	float intensity = splatterData.getIntensity();

	Uint8 newRed = minRedValue + std::rand() % (maxRedValue - minRedValue + 1);
	Uint8 newGreen = minGreenValue + std::rand() % (maxGreenValue - minGreenValue + 1);
	Uint8 newBlue = minBlueValue + std::rand() % (maxBlueValue - minBlueValue + 1);
	Uint8 newAlpha = minAlphaValue + std::rand() % (maxAlphaValue - minAlphaValue + 1);

	// Lock the surface to access the pixel data
	SDL_LockSurface(surface);
	for (int y = top; y < bottom; ++y)
	{
		for (int x = left; x < right; ++x)
		{
			const auto currentPixelPos = Vector2D(static_cast<float>(x), static_cast<float>(y));

			auto distance = Vector2D::Distance(currentPixelPos, splatterCenter);
			if (distance < splatterRadius)
			{
				// Calculate the probability based on the distance
				float probability = static_cast<float>(std::rand()) / RAND_MAX;
				float distanceFactor = 1.0f - (distance / splatterRadius); // Decreasing factor based on distance

				// Adjust the probability based on the distance factor
				if (probability > (intensity * distanceFactor)) continue;

				Uint32 pixel = get_pixel(surface, x, y);
				Uint8 r, g, b, a;
				SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

				if (a == newAlpha) continue;

				r = newRed;
				g = newGreen;
				b = newBlue;
				a = newAlpha;

				pixel = SDL_MapRGBA(surface->format, r, g, b, a);
				set_pixel(surface, x, y, pixel);
			}
		}
	}
	// Unlock the surface after modifying the pixel data
	SDL_UnlockSurface(surface);
}

SDL_Texture* TextureManager::getTextureFromSurface(SDL_Surface* surface, const std::optional<SDL_Color>& textureColor)
{
	if (textureColor)
	{
		Uint32* pixels = (Uint32*)surface->pixels;
		int pixelCount = (surface->w * surface->h);
		for (int i = 0; i < pixelCount; i++) {
			Uint32 pixel = pixels[i];
			Uint8 alpha = (pixel >> 24) & 0xFF;
			if (alpha != 0) { // non-transparent pixel
				pixels[i] = SDL_MapRGBA(surface->format, textureColor->r, textureColor->g, textureColor->b, textureColor->a);
			}
		}
	}
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
		if (surface == nullptr) continue;

		// if this is the first surface, create the combined surface to match its size and format
		if (combinedSurface == nullptr)
		{
			combinedSurface = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->format);
			if (combinedSurface == nullptr)
			{
				throw std::runtime_error("Failed to create combined surface: " + std::string(SDL_GetError()));
			}
		}
		if (SDL_BlitSurface(surface, nullptr, combinedSurface, nullptr) != 0)
		{
			throw std::runtime_error("Failed to blit surface: " + std::string(SDL_GetError()));
		}
	}

	if (combinedSurface != nullptr)
	{
		texture = getTextureFromSurface(combinedSurface, textureColor);
	}

	// free the combined surface
	if (combinedSurface != nullptr)
	{
		SDL_FreeSurface(combinedSurface);
	}

	return texture;
}

void TextureManager::draw(SDL_Texture* texture, const SDL_Rect source, SDL_Rect destination, const double angle, const SDL_RendererFlip flip, const bool moveWithCamera)
{
	// TODO: redo with std::optional ?
	if (moveWithCamera)
	{
		destination.x -= Game::cameraManager->getCameraPosition().x;
		destination.y -= Game::cameraManager->getCameraPosition().y;
	}

	if (Game::cameraManager->isInView(destination))
		SDL_RenderCopyEx(Game::renderer, texture, &source, &destination, angle, NULL, flip);
}

void TextureManager::draw(SDL_Texture* texture, const SDL_Rect* source, SDL_Rect* destination, const double angle, const SDL_RendererFlip flip, const bool moveWithCamera)
{
	// TODO: redo with std::optional ?
	if (moveWithCamera)
	{
		destination->x -= Game::cameraManager->getCameraPosition().x;
		destination->y -= Game::cameraManager->getCameraPosition().y;
	}

	if (Game::cameraManager->isInView(*destination))
		SDL_RenderCopyEx(Game::renderer, texture, source, destination, angle, NULL, flip);
}

Uint32 TextureManager::get_pixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	switch (bpp)
	{
	case 1:
		return *p;
	case 2:
		return *(Uint16*)p;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *(Uint32*)p;
	default:
		return 0;
	}
}

void TextureManager::set_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	switch (bpp)
	{
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16*)p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else
		{
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32*)p = pixel;
		break;
	}
}