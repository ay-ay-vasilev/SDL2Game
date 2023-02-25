#pragma once
#include "Components.h"
#include "Map.h"

class MapSystem : public System
{
public:
	MapSystem(Manager& manager) : System(manager) {}

	void instantiateMap(const std::string textureId, const int tileSize, const std::string mapFilePath, const int mapWidth, const int mapHeight);

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> tiles;
	std::unique_ptr<Map> map;
};