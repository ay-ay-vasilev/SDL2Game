#pragma once
#include "Components.h"

class HitBoxSystem : public System
{
public:
	HitBoxSystem(Manager& manager) : System(manager) {}

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> players;
	std::vector<Entity*> enemies;
	std::vector<Entity*> projectiles;
};