#pragma once
#include "ECS.h"
#include "Map.h"

namespace ecs
{
	class MapSystem : public System
	{
	public:
		explicit MapSystem(Manager& manager) : System(manager) {}

		void instantiateMap(const std::string surfaceId, const int tileSize, const std::string mapFilePath, const int mapWidth, const int mapHeight);

		void update(double delta) override;
		void draw() override;

	private:
		std::vector<Entity*> tiles;
		std::unique_ptr<Map> map;
	};
}
