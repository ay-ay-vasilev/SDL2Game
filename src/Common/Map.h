#pragma once
#include <string>

class Map
{
public:
	Map(const std::string_view textureId, const int mapScale, const int tileSize);
	~Map();

	void LoadMap(const std::string_view path, const int sizeX, const int sizeY);
	void AddTile(const int srcX, const int srcY, const int xpos, const int ypos);
private:
	void LoadTiles(const std::string_view path, const int sizeX, const int sizeY);
	void LoadCollisions(const std::string_view path, const int sizeX, const int sizeY);

	std::string textureId;
	int mapScale;
	int tileSize;
	int scaledSize;
};