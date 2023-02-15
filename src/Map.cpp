#include "Map.h"
#include "Game.h"
#include "ECS.h"
#include "Components.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>

extern Manager manager;

Map::Map(std::string textureId, int mapScale, int tileSize) :
	textureId(textureId),
	mapScale(mapScale),
	tileSize(tileSize),
	scaledSize(mapScale* tileSize)
{
}

Map::~Map()
{
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	LoadTiles(path, sizeX, sizeY);
	LoadCollisions(path, sizeX, sizeY);
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, textureId);
	tile.addGroup(Game::eGroupLabels::MAP);
}

void Map::LoadTiles(std::string path, int sizeX, int sizeY)
{
	std::fstream mapFile;
	mapFile.open("../assets/map/" + path + "_Tiles.csv");

	int srcX, srcY;
	int tileSetCols = 6;

	int x = 0;
	int y = 0;
	std::string line;
	while (std::getline(mapFile, line))
	{
		x = 0;
		std::istringstream lineStream(line);
		int tileNum;
		while (lineStream >> tileNum)
		{
			std::cout << tileNum << "\t";
			srcX = (tileNum % tileSetCols) * tileSize;
			srcY = (tileNum / tileSetCols) * tileSize;
			AddTile(srcX, srcY, x * scaledSize, y * scaledSize);

			if (lineStream.peek() == ',')
			{
				lineStream.ignore();
			}
			x++;
		}
		std::cout << "\n";
		y++;
	}

	mapFile.close();
}

void Map::LoadCollisions(std::string path, int sizeX, int sizeY)
{
	std::fstream mapFile;
	mapFile.open("../assets/map/" + path + "_Collision.csv");

	int srcX, srcY;
	int tileSetCols = 6;

	int x = 0;
	int y = 0;
	std::string line;
	std::cout << "\n";
	while (std::getline(mapFile, line))
	{
		x = 0;
		std::istringstream lineStream(line);
		int tileNum;
		while (lineStream >> tileNum)
		{
			std::cout << tileNum << "\t";
			if (tileNum == 1)
			{
				auto & tileCollider(manager.addEntity());
				tileCollider.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tileCollider.addGroup(Game::COLLIDERS);
			}

			if (tileNum == 0 || lineStream.peek() == ',')
			{
				lineStream.ignore();
			}
			x++;
		}
		std::cout << "\n";
		y++;
	}

	mapFile.close();
}