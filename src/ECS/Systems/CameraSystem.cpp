#include "CameraSystem.h"
#include "TransformComponent.h"
#include "Game.h"

ecs::CameraSystem::CameraSystem(Manager& manager) : System(manager), camera(), cameraBounds(), lastPlayerPosition(0, 0) {}

void ecs::CameraSystem::init()
{
	camera =
	{
		0, 0,
		manager.getConstants()->SCREEN_WIDTH,
		manager.getConstants()->SCREEN_HEIGHT
	};

	const auto mapWidth = manager.getConstants()->MAP_TILE_WIDTH * manager.getConstants()->TILE_SIZE * manager.getConstants()->SCALE;
	const auto mapHeight = manager.getConstants()->MAP_TILE_HEIGHT * manager.getConstants()->TILE_SIZE * manager.getConstants()->SCALE;

	cameraBounds.x = std::min(0, static_cast<int>(-(manager.getConstants()->SCREEN_WIDTH - mapWidth) / 2));
	cameraBounds.y = std::min(0, static_cast<int>(-(manager.getConstants()->SCREEN_HEIGHT - mapHeight) / 2));
	cameraBounds.w = std::max(0, static_cast<int>(mapWidth - manager.getConstants()->SCREEN_WIDTH));
	cameraBounds.h = std::max(0, static_cast<int>(mapHeight - manager.getConstants()->SCREEN_HEIGHT));
}

void ecs::CameraSystem::update(double delta)
{
	players = manager.getGroup(Game::eGroupLabels::PLAYERS);
	if (!players.empty())
		lastPlayerPosition = (*players.begin())->getComponent<ecs::TransformComponent>()->getPosition();

	camera.x = std::clamp(static_cast<int>(lastPlayerPosition.x - manager.getConstants()->SCREEN_WIDTH / 2), cameraBounds.x, cameraBounds.x + cameraBounds.w);
	camera.y = std::clamp(static_cast<int>(lastPlayerPosition.y - manager.getConstants()->SCREEN_HEIGHT / 2), cameraBounds.y, cameraBounds.y + cameraBounds.h);
}

void ecs::CameraSystem::draw()
{
}