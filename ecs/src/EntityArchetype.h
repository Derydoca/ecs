#pragma once
#include "BlockAllocator.h"
#include "Entity.h"
#include <array>
#include "TypeIdentifier.h"

namespace ECS
{
	const int MAX_TYPE_IDENTIFIER_COUNT = 16;

	enum EntityArchetypeComparisonFlags
	{
		None = 0,
		IgnoreOrder = 1
	};

	struct EntityArchetype
	{
	public:
		const static EntityArchetype Empty;

		EntityArchetype(
			TID type00 = NULL_TID,
			TID type01 = NULL_TID,
			TID type02 = NULL_TID,
			TID type03 = NULL_TID,
			TID type04 = NULL_TID,
			TID type05 = NULL_TID,
			TID type06 = NULL_TID,
			TID type07 = NULL_TID,
			TID type08 = NULL_TID,
			TID type09 = NULL_TID,
			TID type10 = NULL_TID,
			TID type11 = NULL_TID,
			TID type12 = NULL_TID,
			TID type13 = NULL_TID,
			TID type14 = NULL_TID,
			TID type15 = NULL_TID
			);

		static EntityArchetype CreateArchetypeWithNewType(
			EntityArchetype archetype,
			TID typeToAppend
		);

		const size_t GetEntitySize() const { return m_entitySize; }
		const size_t GetTypeOffset(TID componentTypeId) const;
		const TID GetType(size_t typeIndex) const { return m_componentTypes[typeIndex]; }

		const bool ArchetypeContainsAllMyTypes(const EntityArchetype& rhs) const
		{
			bool found = false;
			for (size_t i = 0; i < MAX_TYPE_IDENTIFIER_COUNT; i++)
			{
				found = false;
				TID thisTid = m_componentTypes[i];
				for (size_t j = 0; j < MAX_TYPE_IDENTIFIER_COUNT; j++)
				{
					if (thisTid == rhs.m_componentTypes[j])
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					return false;
				}
			}
			return true;
		}

		const bool Equals(const EntityArchetype& rhs, EntityArchetypeComparisonFlags flags = EntityArchetypeComparisonFlags::None) const {
			if (flags & EntityArchetypeComparisonFlags::IgnoreOrder)
			{
				return ArchetypeContainsAllMyTypes(rhs) && rhs.ArchetypeContainsAllMyTypes(*this);
			}
			else
			{
				return operator==(rhs);
			}
		}

		const bool operator==(const EntityArchetype& rhs) const
		{
			for (size_t i = 0; i < MAX_TYPE_IDENTIFIER_COUNT; i++)
			{
				if (m_componentTypes[i] != rhs.m_componentTypes[i])
				{
					return false;
				}
			}
			return true;
		}

		const bool operator!=(const EntityArchetype& rhs) const
		{
			return !operator==(rhs);
		}
	private:
		TID m_componentTypes[MAX_TYPE_IDENTIFIER_COUNT];
		size_t m_entitySize;
	};

}
