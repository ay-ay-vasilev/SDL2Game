#pragma once
#include "ECS.h"

class FactionComponent : public Component
{
public:
	explicit FactionComponent(std::string faction) : faction(faction) {}
	const std::string inline getFaction() const { return faction; }

private:
	std::string faction;
};