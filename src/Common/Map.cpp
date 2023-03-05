#include "Map.h"
#include "Game.h"
#include "ECS.h"
#include "Components.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>

Map::Map(const std::string_view& textureId, const int tileSize) :
	textureId(textureId),
	tileSize(tileSize),
	scaledSize(static_cast<int>(Game::manager->getScale()) * tileSize)
{
}

Map::~Map()
{
}

void Map::loadMap(const std::string& path, const int sizeX, const int sizeY)
{
	loadTiles(path, sizeX, sizeY);
	loadCollisions(path, sizeX, sizeY);
}

void Map::addTile(const int srcX, const int srcY, const int xpos, const int ypos)
{
	auto& tile(Game::manager->addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, textureId);
	tile.addGroup(Game::eGroupLabels::MAP);
}

void Map::loadTiles(const std::string& path, const int sizeX, const int sizeY)
{
	const std::string tempPath(path);
	std::fstream mapFile;
	mapFile.open("../assets/map/" + tempPath + "_tiles.csv");

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
			addTile(srcX, srcY, x * scaledSize, y * scaledSize);

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

void Map::loadCollisions(const std::string& path, const int sizeX, const int sizeY)
{
	const std::string tempPath(path);
	std::fstream mapFile;
	mapFile.open("../assets/map/" + tempPath + "_collision.csv");

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
				auto & tileCollider(Game::manager->addEntity());
				tileCollider.addComponent<ColliderComponent>("terrain", Vector2D(x * scaledSize + scaledSize / 2, y * scaledSize + scaledSize / 2), scaledSize, scaledSize);
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