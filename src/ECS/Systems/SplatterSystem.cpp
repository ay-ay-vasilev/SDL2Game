#include "SplatterSystem.h"
#include "SplatterComponent.h"
#include "TileComponent.h"

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
			applySplatterToTile(splatterComponent->getSplatterData().splatterPosition);
			splatterComponent->setNeedToCreateSplatter(false);
		}
	}
}

void ecs::SplatterSystem::applySplatterToTile(Vector2D pos)
{
	auto tileEntities = manager.getEntitiesWithComponent<ecs::TileComponent>();
	// Calculate the tile index based on the position
	int tileIndexX = static_cast<int>(pos.x / (Game::constants->TILE_SIZE * Game::constants->SCALE));
	int tileIndexY = static_cast<int>(pos.y / (Game::constants->TILE_SIZE * Game::constants->SCALE));

	int mapWidth = Game::constants->MAP_TILE_WIDTH;
	int mapHeight = Game::constants->MAP_TILE_HEIGHT;

	int tileIndex = tileIndexY * mapWidth + tileIndexX;
	const auto tileEntity = tileEntities[tileIndex];

	if (tileEntity)
	{
		auto tileComponent = tileEntity->getComponent<ecs::TileComponent>();
		tileComponent->applySplatter(pos, 5);
	}
}