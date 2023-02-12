#pragma once
#include <string>

class Map
{
public:
	Map(std::string mapFilePath, int mapScale, int tileSize);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int xpos, int ypos);
private:
	void LoadTiles(std::string path, int sizeX, int sizeY);
	void LoadCollisions(std::string path, int sizeX, int sizeY);

	std::string mapFilePath;
	int mapScale;
	int tileSize;
	int scaledSize;
};