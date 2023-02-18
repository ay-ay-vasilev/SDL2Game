#pragma once
#include <SDL_pixels.h>
#include <json.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <map>

class Constants
{
public:
	Constants(const std::string& filename)
	{
		std::ifstream file(filename);
		if (!file.is_open())
		{
			std::cerr << "Failed to open constants file: " << filename << std::endl;
			return;
		}

		nlohmann::json constantsJson;
		file >> constantsJson;

		LoadFromJsonObject(constantsJson);
	}

	// game settings
	int SCREEN_WIDTH{ 800 };
	int SCREEN_HEIGHT{ 640 };
	bool FULLSCREEN{ false };
	float SCALE{ 6.f };
	std::string WINDOW_TITLE{ "Game Window" };

	// map settings - todo: maps will have different sizes
	int MAP_TILE_WIDTH{ 16 };
	int MAP_TILE_HEIGHT{ 16 };
	int TILE_SIZE{ 32 };

	// projectile settings - todo: projectiles will have different size and range
	int PROJECTILE_SIZE{ 32 };
	int PROJECTILE_RANGE{ 200 };

	// colors
	SDL_Color WHITE{ 255, 255, 255, 255 };

	// fonts
	int DEBUG_FONT_SIZE{ 16 };

	// player
	int PLAYER_WIDTH{ 16 };
	int PLAYER_HEIGHT{ 16 };
	int PLAYER_SPEED{ 5 };

private:

	// helper function to load constants from JSON object
	void LoadFromJsonObject(const nlohmann::json& constantsJson)
	{
		if (constantsJson.find("SCREEN_WIDTH") != constantsJson.end())
		{
			SCREEN_WIDTH = constantsJson["SCREEN_WIDTH"];
		}
		if (constantsJson.find("SCREEN_HEIGHT") != constantsJson.end())
		{
			SCREEN_HEIGHT = constantsJson["SCREEN_HEIGHT"];
		}
		if (constantsJson.find("FULLSCREEN") != constantsJson.end())
		{
			FULLSCREEN = constantsJson["FULLSCREEN"];
		}
		if (constantsJson.find("SCALE") != constantsJson.end())
		{
			SCALE = constantsJson["SCALE"];
		}
		if (constantsJson.find("SCALE") != constantsJson.end())
		{
			SCALE = constantsJson["SCALE"];
		}
		if (constantsJson.find("WINDOW_TITLE") != constantsJson.end())
		{
			WINDOW_TITLE = constantsJson["WINDOW_TITLE"];
		}

		if (constantsJson.find("MAP_TILE_WIDTH") != constantsJson.end())
		{
			MAP_TILE_WIDTH = constantsJson["MAP_TILE_WIDTH"];
		}
		if (constantsJson.find("MAP_TILE_HEIGHT") != constantsJson.end())
		{
			MAP_TILE_HEIGHT = constantsJson["MAP_TILE_HEIGHT"];
		}
		if (constantsJson.find("TILE_SIZE") != constantsJson.end())
		{
			TILE_SIZE = constantsJson["TILE_SIZE"];
		}
		if (constantsJson.find("PROJECTILE_SIZE") != constantsJson.end())
		{
			PROJECTILE_SIZE = constantsJson["PROJECTILE_SIZE"];
		}
		if (constantsJson.find("PROJECTILE_RANGE") != constantsJson.end())
		{
			PROJECTILE_RANGE = constantsJson["PROJECTILE_RANGE"];
		}
		if (constantsJson.find("WHITE") != constantsJson.end())
		{
			auto white = constantsJson["WHITE"];
			WHITE.r = white[0];
			WHITE.g = white[1];
			WHITE.b = white[2];
			WHITE.a = white[3];
		}
		if (constantsJson.find("DEBUG_FONT_SIZE") != constantsJson.end())
		{
			DEBUG_FONT_SIZE = constantsJson["DEBUG_FONT_SIZE"];
		}
		if (constantsJson.find("PLAYER_WIDTH") != constantsJson.end())
		{
			PLAYER_WIDTH = constantsJson["PLAYER_WIDTH"];
		}
		if (constantsJson.find("PLAYER_HEIGHT") != constantsJson.end())
		{
			PLAYER_HEIGHT = constantsJson["PLAYER_HEIGHT"];
		}
		if (constantsJson.find("PLAYER_SPEED") != constantsJson.end())
		{
			PLAYER_SPEED = constantsJson["PLAYER_SPEED"];
		}
	}
};