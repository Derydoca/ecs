#include "pch.h"
#include "entities.h"

Entity::Entity()
{
	static int nextId = 0;
	id = nextId++;
}
