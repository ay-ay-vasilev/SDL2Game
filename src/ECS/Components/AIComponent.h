#pragma once
#include "ECS.h"

class AIComponent : public Component
{
public:
	AIComponent() {}
	virtual ~AIComponent() {}
	virtual void update(Entity& entity) {}
};