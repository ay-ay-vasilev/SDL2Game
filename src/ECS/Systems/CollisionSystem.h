#pragma once
#include "Components.h"

class CollisionSystem : public System
{
public:
	CollisionSystem(Manager& manager) : System(manager) {}

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> movingColliders;
	std::vector<Entity*> colliders;
};