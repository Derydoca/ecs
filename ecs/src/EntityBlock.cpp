#include "pch.h"
#include "EntityBlock.h"
#include <cassert>

namespace ECS
{
	namespace Memory
	{

		EntityBlock::EntityBlock(MemoryBlockDescriptor blockDescriptor, EntityArchetype archetype) :
			m_blockDescriptor(blockDescriptor),
			m_archetype(archetype),
			m_maxEntityCount(0)
		{
			size_t blockSize = blockDescriptor.m_blockSize;
			size_t entitySize = archetype.GetEntitySize();
			size_t headerOffset = blockSize / entitySize;
			m_maxEntityCount = (blockSize - headerOffset) / entitySize;
		}

		void EntityBlock::InsertEntityData(const int entityIndex, const char* data)
		{
			assert(entityIndex < m_maxEntityCount);
			memcpy(GetEntityMemoryAddress(entityIndex), data, m_archetype.GetEntitySize());
		}

		void EntityBlock::DeleteEntityData(const int entityIndex)
		{
			assert(entityIndex < m_maxEntityCount);
			memset(GetEntityMemoryAddress(entityIndex), 0, m_archetype.GetEntitySize());
		}

		char* EntityBlock::GetEntityMemoryAddress(const int entityIndex)
		{
			assert(entityIndex < m_maxEntityCount);
			return &m_blockDescriptor.m_data[entityIndex * m_archetype.GetEntitySize()];
		}

	}
}
