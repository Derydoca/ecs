#include "pch.h"
#include "EntityBlock.h"

namespace ECS
{
	namespace Memory
	{

		EntityBlock::EntityBlock(MemoryBlockDescriptor blockDescriptor, EntityArchetype archetype) :
			m_blockDescriptor(blockDescriptor),
			m_archetype(archetype),
			m_maxEntityCount(0)
		{
			m_maxEntityCount = blockDescriptor.m_blockSize / archetype.GetEntitySize();
		}

	}
}
