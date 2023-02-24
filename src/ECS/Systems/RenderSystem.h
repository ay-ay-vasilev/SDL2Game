#pragma once
#include "Components.h"
#include "Map.h"

class RenderSystem : public System
{
public:
	void init() override;
	void update() override
	{
		players = Game::manager->getGroup(Game::eGroupLabels::PLAYERS);
		enemies = Game::manager->getGroup(Game::eGroupLabels::ENEMIES);
		tiles = Game::manager->getGroup(Game::eGroupLabels::MAP);
		projectiles = Game::manager->getGroup(Game::eGroupLabels::PROJECTILES);
	}

	void draw() override;

private:
	std::vector<Entity*> tiles;
	std::vector<Entity*> enemies;
	std::vector<Entity*> players;
	std::vector<Entity*> projectiles;
};