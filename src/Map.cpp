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

Map::Map(const std::string_view textureId, const int mapScale, const int tileSize) :
	textureId(textureId),
	mapScale(mapScale),
	tileSize(tileSize),
	scaledSize(mapScale* tileSize)
{
}

Map::~Map()
{
}

void Map::LoadMap(const std::string_view path, const int sizeX, const int sizeY)
{
	LoadTiles(path, sizeX, sizeY);
	LoadCollisions(path, sizeX, sizeY);
}

void Map::AddTile(const int srcX, const int srcY, const int xpos, const int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, textureId);
	tile.addGroup(Game::eGroupLabels::MAP);
}

void Map::LoadTiles(const std::string_view path, const int sizeX, const int sizeY)
{
	const std::string tempPath(path);
	std::fstream mapFile;
	mapFile.open("../assets/map/" + tempPath + "_Tiles.csv");

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

void Map::LoadCollisions(const std::string_view path, const int sizeX, const int sizeY)
{
	const std::string tempPath(path);
	std::fstream mapFile;
	mapFile.open("../assets/map/" + tempPath + "_Collision.csv");

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