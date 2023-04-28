#include "ECS.h"

void ecs::Entity::addGroup(Group group)
{
	groupBitSet[group] = true;
	manager.addToGroup(this, group);
}