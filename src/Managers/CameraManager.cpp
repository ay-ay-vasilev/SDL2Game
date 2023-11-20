#include "CameraManager.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Constants.h"

CameraManager::CameraManager(std::shared_ptr<ecs::Manager> manager) : camera(), cameraBounds(), manager(manager), screenWidth(), screenHeight() {}

CameraManager::~CameraManager() {}

void CameraManager::init()
{
	auto& constants = constants::Constants::Instance();

	screenWidth = constants::GetConstant<int>(constants, "screen_width");
	screenHeight = constants::GetConstant<int>(constants, "screen_height");
	const auto& MAP_TILE_WIDTH = constants::GetConstant<int>(constants, "map_tile_width");
	const auto& MAP_TILE_HEIGHT = constants::GetConstant<int>(constants, "map_tile_height");
	const auto& TILE_SIZE = constants::GetConstant<int>(constants, "tile_size");
	const auto& SCALE = constants::GetConstant<float>(constants, "scale");

	camera = { 0, 0, screenWidth, screenHeight};

	const auto mapWidth = MAP_TILE_WIDTH * TILE_SIZE * SCALE;
	const auto mapHeight = MAP_TILE_HEIGHT * TILE_SIZE * SCALE;

	cameraBounds.x = std::min(0, static_cast<int>(-(screenWidth - mapWidth) / 2));
	cameraBounds.y = std::min(0, static_cast<int>(-(screenHeight - mapHeight) / 2));
	cameraBounds.w = std::max(0, static_cast<int>(mapWidth - screenWidth));
	cameraBounds.h = std::max(0, static_cast<int>(mapHeight - screenHeight));
}

void CameraManager::update()
{
	cameraEntities = manager->getEntitiesWithComponent<ecs::CameraComponent>();

	if (cameraEntities.empty()) return;

	Vector2D cameraPosition = getSumCameraPosition();

	camera.x = std::clamp(static_cast<int>(cameraPosition.x - screenWidth / 2), cameraBounds.x, cameraBounds.x + cameraBounds.w);
	camera.y = std::clamp(static_cast<int>(cameraPosition.y - screenHeight / 2), cameraBounds.y, cameraBounds.y + cameraBounds.h);

	updatePositionsOnScreen();
}

const bool CameraManager::isInView(const SDL_Rect& destination) const
{
	return (destination.x < (camera.x + camera.w + destination.w) && destination.x >= ((-1) * destination.w))
		&& (destination.y < (camera.y + camera.h + destination.h) && destination.y >= ((-1) * destination.h));
}

const Vector2D CameraManager::getSumCameraPosition() const
{
	Vector2D sumPosition;
	int entityCount = 0;
	for (auto entity : cameraEntities)
	{
		if (entity->hasComponent<ecs::TransformComponent>())
		{
			sumPosition += entity->getComponent<ecs::TransformComponent>()->getPosition();
			entityCount++;
		}
	}
	return sumPosition / static_cast<float>(entityCount);
}

void CameraManager::updatePositionsOnScreen() const
{
	for (auto entity : cameraEntities)
	{
		if (entity->hasComponent<ecs::TransformComponent>())
		{
			const auto entityPosition = entity->getComponent<ecs::TransformComponent>()->getPosition();
			const Vector2D positionOnScreen{ entityPosition.x - camera.x, entityPosition.y - camera.y };
			entity->getComponent<ecs::CameraComponent>()->setPositionOnScreen(positionOnScreen);
		}
	}
}