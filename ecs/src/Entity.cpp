#include "pch.h"
#include "Entity.h"

namespace ECS
{

	const int Entity::INVALID_ENTITY_ID = -1;

	Entity::Entity(int id) :
		m_id(id)
	{
	}

}
