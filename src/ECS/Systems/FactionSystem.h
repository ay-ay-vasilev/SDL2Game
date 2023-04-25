#pragma once
#include "ECS.h"

class FactionSystem : public System
{
public:
	explicit FactionSystem(Manager& manager) : System(manager) {}
	// System
	void init() override;
	void update() override;
	void draw() override;

private:
	std::unordered_map<std::string, std::unordered_map<std::string, int>> factionRelationships;
};