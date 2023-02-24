#pragma once
#include "Components.h"

class EnemySystem : public System
{
public:
	EnemySystem(Manager& manager) : System(manager) {}

	void instantiateEnemy(const Vector2D pos, const Vector2D size, const float scale, const int speed, const std::string_view id) const;

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> enemies;
};