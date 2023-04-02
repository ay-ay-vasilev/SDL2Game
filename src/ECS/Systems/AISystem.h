#pragma once
#include "ECS.h"

class AISystem : public System
{
public:
	AISystem(Manager& manager) : System(manager) {}

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> basicEnemies;
	std::vector<Entity*> players;
};