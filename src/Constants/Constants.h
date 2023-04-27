#pragma once
#include <SDL_pixels.h>
#pragma warning(push, 0)
#include <wrappedJson.h>
#pragma warning(pop)

#include "Vector2D.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>

class Constants
{
public:
	Constants(const std::string& filename) : settingsFile(filename)
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

	void ReloadSettings()
	{
		std::ifstream file(settingsFile);
		if (!file.is_open())
		{
			std::cerr << "Failed to open constants file: " << settingsFile << std::endl;
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

	// start positions
	Vector2D PLAYER_POS{200, 200};
	std::vector<Vector2D> HUMAN_POS;
	std::vector<Vector2D> SKELETON_POS;
	std::vector<Vector2D> GOBLIN_POS;

	bool DRAW_COLLIDERS{ false };
	bool DRAW_HITBOXES{ false };

private:

	// helper function to load constants from JSON object
	void LoadFromJsonObject(const nlohmann::json& constantsJson)
	{
		if (constantsJson.find("screen_width") != constantsJson.end())
		{
			SCREEN_WIDTH = constantsJson["screen_width"];
		}
		if (constantsJson.find("screen_height") != constantsJson.end())
		{
			SCREEN_HEIGHT = constantsJson["screen_height"];
		}
		if (constantsJson.find("fullscreen") != constantsJson.end())
		{
			FULLSCREEN = constantsJson["fullscreen"];
		}
		if (constantsJson.find("scale") != constantsJson.end())
		{
			SCALE = constantsJson["scale"];
		}
		
		if (constantsJson.find("window_title") != constantsJson.end())
		{
			WINDOW_TITLE = constantsJson["window_title"];
		}

		if (constantsJson.find("map_tile_width") != constantsJson.end())
		{
			MAP_TILE_WIDTH = constantsJson["map_tile_width"];
		}
		if (constantsJson.find("map_tile_height") != constantsJson.end())
		{
			MAP_TILE_HEIGHT = constantsJson["map_tile_height"];
		}
		if (constantsJson.find("tile_size") != constantsJson.end())
		{
			TILE_SIZE = constantsJson["tile_size"];
		}
		if (constantsJson.find("projectile_size") != constantsJson.end())
		{
			PROJECTILE_SIZE = constantsJson["projectile_size"];
		}
		if (constantsJson.find("projectile_range") != constantsJson.end())
		{
			PROJECTILE_RANGE = constantsJson["projectile_range"];
		}
		if (constantsJson.find("white") != constantsJson.end())
		{
			auto white = constantsJson["white"];
			WHITE.r = white[0];
			WHITE.g = white[1];
			WHITE.b = white[2];
			WHITE.a = white[3];
		}
		if (constantsJson.find("debug_font_size") != constantsJson.end())
		{
			DEBUG_FONT_SIZE = constantsJson["debug_font_size"];
		}

		if (constantsJson.find("player_pos") != constantsJson.end())
		{
			PLAYER_POS.x = constantsJson["player_pos"]["x"];
			PLAYER_POS.y = constantsJson["player_pos"]["y"];
		}
		
		if (constantsJson.find("human_positions") != constantsJson.end())
		{
			for (const auto& posData : constantsJson["human_positions"])
			{
				HUMAN_POS.push_back({ posData["x"], posData["y"] });
			}
		}
		if (constantsJson.find("skeleton_positions") != constantsJson.end())
		{
			for (const auto& posData : constantsJson["skeleton_positions"])
			{
				SKELETON_POS.push_back({ posData["x"], posData["y"] });
			}
		}
		if (constantsJson.find("goblin_positions") != constantsJson.end())
		{
			for (const auto& posData : constantsJson["goblin_positions"])
			{
				GOBLIN_POS.push_back({ posData["x"], posData["y"] });
			}
		}

		if (constantsJson.find("debug_draw_colliders") != constantsJson.end())
		{
			DRAW_COLLIDERS = constantsJson["debug_draw_colliders"];
		}
		if (constantsJson.find("debug_draw_hitboxes") != constantsJson.end())
		{
			DRAW_HITBOXES = constantsJson["debug_draw_hitboxes"];
		}
	}

	std::string settingsFile;
};