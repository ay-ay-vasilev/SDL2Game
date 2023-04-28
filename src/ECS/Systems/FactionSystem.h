#pragma once
#include "ECS.h"

namespace ecs
{
	class FactionSystem : public System
	{
	public:
		explicit FactionSystem(Manager& manager) : System(manager) {}
		// System
		void init() override;
		void update() override;
		void draw() override;

		void updateRelations();

	private:
		std::unordered_map<std::string, std::unordered_map<std::string, int>> factions;
	};
}
