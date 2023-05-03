#include "MapSystem.h"
#include "Game.h"

void ecs::MapSystem::instantiateMap(const std::string surfaceId, const int tileSize, const std::string mapFilePath, const int mapWidth, const int mapHeight)
{
	map = std::make_unique<Map>(surfaceId, tileSize);
	map->loadMap("map", mapWidth, mapHeight);
}

void ecs::MapSystem::update(double delta)
{
	tiles = manager.getGroup(Game::eGroupLabels::MAP);
}

void ecs::MapSystem::draw()
{
}