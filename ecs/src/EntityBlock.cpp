#include "pch.h"
#include "EntityBlock.h"
#include <cassert>

namespace ECS
{
	namespace Memory
	{
		EntityBlock::EntityBlock() :
			m_blockDescriptor(),
			m_archetype(),
			m_maxEntityCount(0)
		{
		}

		EntityBlock::EntityBlock(MemoryBlockDescriptor blockDescriptor, EntityArchetype archetype) :
			m_blockDescriptor(blockDescriptor),
			m_archetype(archetype),
			m_maxEntityCount(0)
		{
			Initialize();
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

		void EntityBlock::Assign(MemoryBlockDescriptor blockDescriptor, EntityArchetype archetype)
		{
			m_blockDescriptor = blockDescriptor;
			m_archetype = archetype;
			Initialize();
		}

		void EntityBlock::Initialize()
		{
			size_t blockSize = m_blockDescriptor.m_blockSize;
			size_t entitySize = m_archetype.GetEntitySize();
			size_t headerOffset = blockSize / entitySize;
			m_maxEntityCount = (blockSize - headerOffset) / entitySize;
		}

	}
}
