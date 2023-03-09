#pragma once
#include "Components.h"

class AIComponent : public Component
{
public:
	AIComponent() {}
	virtual ~AIComponent() {}
	virtual void update(Entity& entity) {}
};