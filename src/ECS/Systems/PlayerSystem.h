#pragma once
#include "Components.h"

class PlayerSystem : public System
{
public:
	PlayerSystem(Manager& manager) : System(manager) {}


	void instantiatePlayer(const Vector2D pos, const Vector2D size, const float scale, const int speed, const std::string_view id) const;
	const Vector2D getPlayerPosition() const;

	void update() override;
	void draw() override;

private:
	std::vector<Entity*> players;
};