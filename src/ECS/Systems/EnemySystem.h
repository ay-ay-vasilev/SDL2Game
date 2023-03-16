#pragma once
#include "Components.h"

class EnemySystem : public System
{
public:
	EnemySystem(Manager& manager) : System(manager) {}

	Entity* instantiateEnemy(const Vector2D& pos, const std::string& filename) const;
	void equipWeapon(Entity& enemy, const std::string& weaponName);

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> enemies;
};