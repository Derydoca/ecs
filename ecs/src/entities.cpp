#include "pch.h"
#include "entities.h"

entity::entity()
{
	static int nextId = 0;
	id = nextId++;
}
