#include "Map.h"
#include "Game.h"
#include <fstream>
#include <iostream>
#include <iterator>

Map::Map()
{
}

Map::~Map()
{
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char tile;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;
	int tileSetCols = 5;

	float scale = 6.f;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile >> tile;
			std::cout << tile << " ";
			int tileNum = (int)tile - '0';

			srcX = (tileNum % tileSetCols) * 32;
			srcY = (tileNum / tileSetCols) * 32;
			Game::AddTile(srcX, srcY, x * 32 * scale, y * 32 * scale);
			mapFile.ignore();
		}
		std::cout << "\n";
	}

	mapFile.close();
}