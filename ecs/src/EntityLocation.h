#pragma once
#include <cassert>

namespace ECS
{

	struct EntityLocation
	{
		const static EntityLocation INVALID_LOCATION;

		EntityLocation() :
			m_data(INVALID_LOCATION.m_data)
		{}

		EntityLocation(unsigned int blockIndex, unsigned int entityIndex)
		{
			Set(blockIndex, entityIndex);
		}

		EntityLocation(unsigned int rawLocationValue) :
			m_data(rawLocationValue)
		{}

		void Set(unsigned int blockIndex, unsigned int entityIndex)
		{
			// Do not allow indices greater than the index mask
			assert((entityIndex >> NUM_INDEX_BITS) == 0);
			m_data = (blockIndex << NUM_INDEX_BITS) | entityIndex;
		}

		void ResetToInvalid()
		{
			m_data = INVALID_LOCATION.m_data;
		}

		unsigned int GetBlockIndex()
		{
			return m_data >> NUM_INDEX_BITS;
		}

		unsigned int GetEntityIndex()
		{
			return (m_data << NUM_ENTITY_BITS) >> NUM_ENTITY_BITS;
		}

		bool operator==(const EntityLocation& rhs) const
		{
			return m_data == rhs.m_data;;
		}

		bool operator!=(const EntityLocation& rhs) const
		{
			return m_data != rhs.m_data;;
		}
	private:
		const int NUM_INDEX_BITS = 4 * 4;
		const int NUM_ENTITY_BITS = 64 - NUM_INDEX_BITS;
		unsigned int m_data;
	};

}
