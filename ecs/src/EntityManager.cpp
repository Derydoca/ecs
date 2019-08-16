#include "pch.h"
#include "EntityManager.h"
#include <cassert>

ECS::EntityManager::EntityManager(const size_t blockSize, const size_t blockCount) :
	m_allocator(blockSize, blockCount),
	m_nextEntityId(1),
	m_blockCount(blockCount),
	m_entityBlocks(0)
{
	m_entityBlocks = new Memory::EntityBlock[m_blockCount];
}

ECS::EntityManager::~EntityManager()
{
	delete[](m_entityBlocks);
}

void ECS::EntityManager::CreateEntity(Entity& entity)
{
	assert(entity.m_id <= 0);
	entity.m_id = m_nextEntityId++;
}

void ECS::EntityManager::CreateEntityWithData(Entity& entity, const EntityArchetype archetype, char* dataPointer)
{
	size_t firstEmptyBlock = -1;
	for (size_t i = 0; i < m_blockCount; i++)
	{
		Memory::EntityBlock entityBlock = m_entityBlocks[i];
		if (entityBlock.GetArchetype() == archetype)
		{
			Entity* entityArray = reinterpret_cast<Entity*>(entityBlock.GetDescriptor().m_data);
			for (size_t j = 0; j < entityBlock.GetMaxEntityCount(); j++)
			{
				Entity entity = entityArray[j];
				if (entity.GetId() != Entity::INVALID_ENTITY_ID)
				{
					entityBlock.InsertEntityData(static_cast<int>(j), dataPointer);
				}
			}
		}
		else if (firstEmptyBlock == -1 && entityBlock.GetDescriptor().m_id == Memory::MemoryBlockDescriptor::INVALID_ID)
		{
			firstEmptyBlock = i;
		}
	}

	if (firstEmptyBlock > -1)
	{
		Memory::EntityBlock newBlock = m_entityBlocks[firstEmptyBlock];
		newBlock.Assign(m_allocator.Allocate(), archetype);
		newBlock.InsertEntityData(0, dataPointer);
	}
	else
	{
		// TODO: Create a new block, increase the entity block array, add entity, when BlockAllocator supports dynamic block counts
	}
}
