#include "CameraManager.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "CameraComponent.h"

CameraManager::CameraManager(std::shared_ptr<ecs::Manager> manager) : manager(manager) {}

CameraManager::~CameraManager() {}

void CameraManager::init()
{
	camera =
	{
		0, 0,
		manager->getConstants()->SCREEN_WIDTH,
		manager->getConstants()->SCREEN_HEIGHT
	};

	const auto mapWidth = manager->getConstants()->MAP_TILE_WIDTH * manager->getConstants()->TILE_SIZE * manager->getConstants()->SCALE;
	const auto mapHeight = manager->getConstants()->MAP_TILE_HEIGHT * manager->getConstants()->TILE_SIZE * manager->getConstants()->SCALE;

	cameraBounds.x = std::min(0, static_cast<int>(-(manager->getConstants()->SCREEN_WIDTH - mapWidth) / 2));
	cameraBounds.y = std::min(0, static_cast<int>(-(manager->getConstants()->SCREEN_HEIGHT - mapHeight) / 2));
	cameraBounds.w = std::max(0, static_cast<int>(mapWidth - manager->getConstants()->SCREEN_WIDTH));
	cameraBounds.h = std::max(0, static_cast<int>(mapHeight - manager->getConstants()->SCREEN_HEIGHT));
}

void CameraManager::update()
{
	cameraEntities = manager->getEntitiesWithComponent<ecs::CameraComponent>();

	if (cameraEntities.empty())
		return;

	Vector2D cameraPosition = getSumCameraPosition();

	camera.x = std::clamp(static_cast<int>(cameraPosition.x - manager->getConstants()->SCREEN_WIDTH / 2), cameraBounds.x, cameraBounds.x + cameraBounds.w);
	camera.y = std::clamp(static_cast<int>(cameraPosition.y - manager->getConstants()->SCREEN_HEIGHT / 2), cameraBounds.y, cameraBounds.y + cameraBounds.h);

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
	return sumPosition / entityCount;
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