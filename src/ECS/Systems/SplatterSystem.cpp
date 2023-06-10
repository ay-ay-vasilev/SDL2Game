#include "SplatterSystem.h"
#include "SplatterComponent.h"
#include "TileSplatterComponent.h"
#include "TileComponent.h"

#include "Game.h"
#include <queue>

void ecs::SplatterSystem::init()
{
}

void ecs::SplatterSystem::update(double delta)
{
	checkForSplatter();
}

void ecs::SplatterSystem::draw()
{
}

void ecs::SplatterSystem::checkForSplatter()
{
	splatterEntities = manager.getEntitiesWithComponent<ecs::SplatterComponent>();

	for (const auto splatterEntity : splatterEntities)
	{
		auto splatterComponent = splatterEntity->getComponent<ecs::SplatterComponent>();

		if (splatterComponent->needToCreateSplatter())
		{
			applySplatterToTile(splatterComponent->getSplatterData().splatterPosition, 10);
			splatterComponent->setNeedToCreateSplatter(false);
		}
	}
}

void ecs::SplatterSystem::applySplatterToTile(Vector2D splatterPos, int splatterRadius)
{
	auto tileSplatterEntities = manager.getEntitiesWithComponent<ecs::TileSplatterComponent>();

	int tileIndexX = static_cast<int>(splatterPos.x / (Game::constants->TILE_SIZE * Game::constants->SCALE));
	int tileIndexY = static_cast<int>(splatterPos.y / (Game::constants->TILE_SIZE * Game::constants->SCALE));

	int mapWidth = Game::constants->MAP_TILE_WIDTH;
	int mapHeight = Game::constants->MAP_TILE_HEIGHT;

	std::queue<int> tileQueue;
	std::vector<bool> visitedTiles(tileSplatterEntities.size(), false);

	int tileIndex = tileIndexY * mapWidth + tileIndexX;
	tileQueue.push(tileIndex);
	visitedTiles[tileIndex] = true;

	while (!tileQueue.empty())
	{
		int currentTileIndex = tileQueue.front();
		tileQueue.pop();

		const auto tileEntity = tileSplatterEntities[currentTileIndex];
		auto tileSplatterComponent = tileEntity->getComponent<ecs::TileSplatterComponent>();
		auto tileComponent = tileEntity->getComponent<ecs::TileComponent>();

		// Check if any of the four vertices of the tile is within the specified radius
		bool isTileInRadius = false;
		const auto tilePosition = tileComponent->getPosition();
		int tileSize = tileComponent->getTileSize() * Game::constants->SCALE;

		const auto topLeft = tilePosition;
		const auto topRight = tilePosition + Vector2D(tileSize, 0);
		const auto bottomLeft = tilePosition + Vector2D(0, tileSize);
		const auto bottomRight = tilePosition + Vector2D(tileSize, tileSize);

		if (Vector2D::Distance(topLeft, splatterPos) <= splatterRadius ||
			Vector2D::Distance(topRight, splatterPos) <= splatterRadius ||
			Vector2D::Distance(bottomLeft, splatterPos) <= splatterRadius ||
			Vector2D::Distance(bottomRight, splatterPos) <= splatterRadius ||
			
			(tilePosition.x <= splatterPos.x && splatterPos.x <= tilePosition.x + tileSize &&
			tilePosition.y <= splatterPos.y && splatterPos.y <= tilePosition.y + tileSize)
			)
		{
			isTileInRadius = true;
		}

		tileSplatterComponent->applySplatter(splatterPos, splatterRadius);

		if (isTileInRadius)
		{
			// Visit the neighboring tiles
			int currentTileX = currentTileIndex % mapWidth;
			int currentTileY = currentTileIndex / mapWidth;

			for (int dx = -1; dx <= 1; ++dx)
			{
				for (int dy = -1; dy <= 1; ++dy)
				{
					int neighborX = currentTileX + dx;
					int neighborY = currentTileY + dy;
					int neighborIndex = neighborY * mapWidth + neighborX;

					// Check if the neighboring tile is valid and hasn't been visited
					if (neighborX >= 0 && neighborX < mapWidth &&
						neighborY >= 0 && neighborY < mapHeight &&
						!visitedTiles[neighborIndex])
					{
						tileQueue.push(neighborIndex);
						visitedTiles[neighborIndex] = true;
					}
				}
			}
		}
	}
}