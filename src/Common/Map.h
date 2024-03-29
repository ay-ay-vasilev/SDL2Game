#pragma once
#include <string>

class Map
{
public:
	Map(const std::string_view& surfaceId, const int tileSize);
	~Map();

	void loadMap(const std::string& path, const int sizeX, const int sizeY);
	void addTile(const int tileNum, const int srcX, const int srcY, const int xpos, const int ypos);
private:
	void loadTiles(const std::string& path, const int sizeX, const int sizeY);
	void loadCollisions(const std::string& path, const int sizeX, const int sizeY);

	std::string surfaceId;
	int tileSize;
	float scaledSize;
};