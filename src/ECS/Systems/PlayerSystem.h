#pragma once
#include "Components.h"

class PlayerSystem : public System
{
public:
	PlayerSystem(Manager& manager) : System(manager) {}

	void instantiatePlayer(const Vector2D& pos, const std::string& filename) const;
	const Vector2D getPlayerPosition() const;
	const int getPlayerWidth() const;
	const int getPlayerHeight() const;

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> players;
};