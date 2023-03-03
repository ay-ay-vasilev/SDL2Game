#pragma once
#include "Components.h"

class HitboxWeaponCollisionSystem : public System
{
public:
	HitboxWeaponCollisionSystem(Manager& manager) : System(manager) {}

	void update() override;
	void draw() override;

private:

	std::vector<Entity*> hitboxes;
	std::vector<Entity*> weapons;
};