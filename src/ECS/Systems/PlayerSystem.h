#pragma once
#include "Components.h"

class PlayerSystem : public System
{
public:
	PlayerSystem(Manager& manager) : System(manager), lastPlayerPosition(0, 0) {}

	Entity* instantiatePlayer(const Vector2D& pos, const std::string& filename);
	void equipWeapon(Entity& player, const std::string& weaponName);

	const Vector2D getPlayerPosition();

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> players;
	Vector2D lastPlayerPosition;
};