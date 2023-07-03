#include "SplatterSystem.h"
#include "SplatterComponent.h"
#include "TileSplatterComponent.h"
#include "TileComponent.h"

#include "Splatter.h"
#include "Game.h"
#include <queue>

ecs::SplatterSystem::~SplatterSystem()
{
	events::EventManager::remove_listener<events::SplatterEvent>(*listenerHandle.get());
}

void ecs::SplatterSystem::init()
{
	listenerHandle = std::make_unique<EventListenerHandle>(events::EventManager::listen<events::SplatterEvent>(this, &ecs::SplatterSystem::createSplatter));
}

void ecs::SplatterSystem::createSplatter(const events::SplatterEvent* splatterEvent)
{
	float minRadius = splatterEvent->radiusRange.first;
	float maxRadius = splatterEvent->radiusRange.second;
	float randomRadius = minRadius + (static_cast<float>(std::rand()) / RAND_MAX) * (maxRadius - minRadius);

	float minOffset = splatterEvent->offsetRange.first;
	float maxOffset = splatterEvent->offsetRange.second;

	// Generate a random offset value between minOffset and maxOffset
	float offsetX = minOffset + (static_cast<float>(std::rand()) / RAND_MAX) * (maxOffset - minOffset);
	float offsetY = minOffset + (static_cast<float>(std::rand()) / RAND_MAX) * (maxOffset - minOffset);
	// Randomly choose the sign for the offset
	if (std::rand() % 2 == 0) offsetX *= -1.f;
	if (std::rand() % 2 == 0) offsetY *= -1.f;

	auto splatterPosition = splatterEvent->splatterPosition;

	auto splatterData = Splatter
	(
		splatterPosition, static_cast<int>(randomRadius), splatterEvent->intensity,
		splatterEvent->redRange, splatterEvent->greenRange, splatterEvent->blueRange, splatterEvent->alphaRange
	);

	applySplatterToTile(splatterData);

	splatterPosition.x += offsetX * Game::constants->SCALE;
	splatterPosition.y += offsetY * Game::constants->SCALE;
	splatterData.setSplatterCenter(splatterPosition);

	applySplatterToTile(splatterData);
}

void ecs::SplatterSystem::applySplatterToTile(const Splatter& splatterData)
{
	auto tileSplatterEntities = manager.getEntitiesWithComponent<ecs::TileSplatterComponent>();
	const auto splatterPos = splatterData.getSplatterCenter();
	const auto splatterRadius = splatterData.getSplatterRadius();

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
		float tileSize = static_cast<float>(tileComponent->getTileSize()) * Game::constants->SCALE;

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

		tileSplatterComponent->applySplatter(splatterData);

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