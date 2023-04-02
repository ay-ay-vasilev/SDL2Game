#include "MapSystem.h"
#include "Game.h"

void MapSystem::instantiateMap(const std::string textureId, const int tileSize, const std::string mapFilePath, const int mapWidth, const int mapHeight)
{
	map = std::make_unique<Map>(textureId, tileSize);
	map->loadMap("map", mapWidth, mapHeight);
}

void MapSystem::update()
{
	tiles = manager.getGroup(Game::eGroupLabels::MAP);
}

void MapSystem::draw()
{
}