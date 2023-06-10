#include "Map.h"
#include "Game.h"
#include "ECS.h"
#include "ColliderComponent.h"
#include "TileComponent.h"
#include "TileSplatterComponent.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>

Map::Map(const std::string_view& surfaceId, const int tileSize) :
	surfaceId(surfaceId),
	tileSize(tileSize),
	scaledSize(Game::manager->getScale() * tileSize)
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
	tile.addComponent<ecs::TileComponent>(srcX, srcY, xpos, ypos, tileSize, surfaceId);
	tile.addComponent<ecs::TileSplatterComponent>();
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
			addTile(srcX, srcY, static_cast<int>(x * scaledSize), static_cast<int>(y * scaledSize));

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
				tileCollider.addComponent<ecs::ColliderComponent>("terrain", Vector2D(static_cast<float>(x * scaledSize + scaledSize / 2), static_cast<float>(y * scaledSize + scaledSize / 2)), scaledSize*1.f, scaledSize*1.f);
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