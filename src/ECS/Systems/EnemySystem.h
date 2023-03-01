#pragma once
#include "Components.h"

class EnemySystem : public System
{
public:
	EnemySystem(Manager& manager) : System(manager) {}

	void instantiateEnemy(const Vector2D& pos, const std::string& filename) const;

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> enemies;
};