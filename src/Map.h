#pragma once
#include "Game.h"

class Map
{
public:
	Map();
	~Map();

	void LoadMap(std::vector<std::vector<int>> levelVector);
	void DrawMap();

private:
	SDL_Rect src, dest;

	SDL_Texture* dirt;
	SDL_Texture* grass;
	SDL_Texture* water;

	std::vector<std::vector<int>> mapVector;
};