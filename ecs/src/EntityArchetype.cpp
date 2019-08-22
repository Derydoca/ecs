#include "pch.h"
#include "EntityArchetype.h"
#include <cassert>

namespace ECS
{

	const EntityArchetype EntityArchetype::Empty = EntityArchetype();

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
			},
		m_entitySize(0)
	{
		for (size_t i = 0; i < MAX_TYPE_IDENTIFIER_COUNT; i++)
		{
			m_entitySize += m_componentTypes[i].GetSize();
		}
	}

	EntityArchetype EntityArchetype::CreateArchetypeWithNewType(EntityArchetype archetype, TID typeToAppend)
	{
		EntityArchetype newArchetype = EntityArchetype(archetype);

		bool completed = false;
		for (size_t i = 0; i < MAX_TYPE_IDENTIFIER_COUNT; i++)
		{
			if(newArchetype.m_componentTypes[i] == NULL_TID)
			{
				newArchetype.m_componentTypes[i] = typeToAppend;
				completed = true;
				break;
			}
		}
		assert(completed);

		return newArchetype;
	}

	const size_t EntityArchetype::GetTypeOffset(TID componentTypeId) const
	{
		size_t offset = 0;
		for (size_t i = 0; i < MAX_TYPE_IDENTIFIER_COUNT; i++)
		{
			TID currentTypeId = m_componentTypes[i];
			if (currentTypeId == componentTypeId)
			{
				return offset;
			}
			else
			{
				offset += currentTypeId.GetSize();
			}
		}
		return -1;
	}

}
