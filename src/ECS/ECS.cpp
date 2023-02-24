#include "ECS.h"

void Entity::addGroup(Group group)
{
	groupBitSet[group] = true;
	manager.addToGroup(this, group);
}