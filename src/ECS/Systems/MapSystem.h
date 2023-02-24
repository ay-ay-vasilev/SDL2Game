#pragma once
#include "Components.h"
#include "Map.h"

class MapSystem : public System
{
public:
	void instantiateMap(const std::string textureId, const int mapScale, const int tileSize, const std::string mapFilePath, const int mapWidth, const int mapHeight);

	void init() override;
	void update() override;
	void draw() override;

private:
	std::vector<Entity*> tiles;
	std::unique_ptr<Map> map;
};