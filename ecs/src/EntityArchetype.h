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
			const EntityArchetype& archetype,
			TID typeToAppend
		);

		const size_t GetEntitySize() const { return m_entitySize; }
		const size_t GetTypeOffset(TID componentTypeId) const;
		const TID GetType(size_t typeIndex) const { return m_componentTypes[typeIndex]; }

		const bool ArchetypeContainsAllMyTypes(const EntityArchetype& rhs) const;
		const bool Equals(const EntityArchetype& rhs, EntityArchetypeComparisonFlags flags = EntityArchetypeComparisonFlags::None) const;

		const bool operator==(const EntityArchetype& rhs) const
		{
			return Equals(rhs, EntityArchetypeComparisonFlags::None);
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
