#pragma once
#include "Components.h"
#include "Game.h"
#include "Collision.h"
#include "Vector2D.h"

class CollisionSystem : public System
{
public:
	void init() override;
	void update() override;
	void draw() override;

private:
	std::vector<Entity*> players;
	std::vector<Entity*> colliders;
	std::vector<Entity*> projectiles;
};