#include "pch.h"
#include "EntityArchetype.h"

namespace ECS
{

	EntityArchetype::EntityArchetype(
		TID type00,
		TID type01,
		TID type02,
		TID type03,
		TID type04,
		TID type05,
		TID type06,
		TID type07,
		TID type08,
		TID type09,
		TID type10,
		TID type11,
		TID type12,
		TID type13,
		TID type14,
		TID type15
	) :
		m_componentTypes{
			type00,
			type01,
			type02,
			type03,
			type04,
			type05,
			type06,
			type07,
			type08,
			type09,
			type10,
			type11,
			type12,
			type13,
			type14,
			type15
			}
	{
	}

}
