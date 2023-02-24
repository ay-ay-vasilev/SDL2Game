#include "MapSystem.h"

void MapSystem::instantiateMap(const std::string textureId, const int mapScale, const int tileSize, const std::string mapFilePath, const int mapWidth, const int mapHeight)
{
	map = std::make_unique<Map>(textureId, mapScale, tileSize);
	map->LoadMap("map", mapWidth, mapHeight);
}

void MapSystem::update()
{
	tiles = manager.getGroup(Game::eGroupLabels::MAP);
}

void MapSystem::draw()
{
}