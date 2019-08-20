#include "pch.h"
#include "EntityManager.h"
#include <cassert>

ECS::EntityManager::EntityManager(const size_t blockSize, const size_t blockCount) :
	m_allocator(blockSize, blockCount),
	m_nextEntityId(1),
	m_blockCount(blockCount),
	m_entityBlocks(0),
	m_maxEntities(1024 * 1024),
	m_entityLocations(0)
{
	m_entityBlocks = new Memory::EntityBlock[m_blockCount];
	m_entityLocations = new EntityLocation[m_maxEntities];
}

ECS::EntityManager::~EntityManager()
{
	delete[](m_entityBlocks);
	delete[](m_entityLocations);
}

void ECS::EntityManager::CreateEntity(Entity& entity)
{
	CreateEntityWithData(entity, EntityArchetype(), nullptr);
}

void ECS::EntityManager::CreateEntityWithData(Entity& entity, const EntityArchetype archetype, char* dataPointer)
{
	assert(entity.m_id <= 0);
	entity.m_id = m_nextEntityId++;
	InsertEntityDataInFirstOpenSlot(entity, EntityArchetype(), dataPointer);
}

void ECS::EntityManager::AddComponentData(Entity entity, TID tid)
{
	EntityLocation sourceLocation = m_entityLocations[entity.GetId()];
	Memory::EntityBlock sourceEntityBlock = m_entityBlocks[sourceLocation.GetBlockIndex()];
	EntityArchetype sourceArchetype = sourceEntityBlock.GetArchetype();
	EntityArchetype destinationArchetype = EntityArchetype::CreateArchetypeWithNewType(sourceArchetype, tid);

	EntityLocation destinationLocation;
	for (unsigned int blockIndex = 0; blockIndex < static_cast<unsigned int>(m_blockCount); blockIndex++)
	{
		Memory::EntityBlock entityBlock = m_entityBlocks[blockIndex];
		if (entityBlock.GetArchetype().Equals(destinationArchetype, ECS::EntityArchetypeComparisonFlags::IgnoreOrder))
		{
			Entity* entityArray = reinterpret_cast<Entity*>(entityBlock.GetDescriptor().m_data);
			for (unsigned int entityIndex = 0; entityIndex < static_cast<unsigned int>(entityBlock.GetMaxEntityCount()); entityIndex++)
			{
				Entity entity = entityArray[entityIndex];
				if (entity.m_id == Entity::INVALID_ENTITY_ID)
				{
					destinationLocation.Set(blockIndex, entityIndex);
				}
			}
		}
	}
	Memory::EntityBlock destinationBlock = m_entityBlocks[destinationLocation.GetBlockIndex()];
	destinationBlock.InsertEntityCopy
	(
		destinationLocation.GetEntityIndex(),
		entity,
		sourceArchetype,
		sourceEntityBlock.GetEntityMemoryAddress(sourceLocation.GetEntityIndex())
	);
	sourceEntityBlock.DeleteEntity(sourceLocation.GetEntityIndex());
}

void ECS::EntityManager::InsertEntityDataInFirstOpenSlot(const Entity entity, const EntityArchetype archetype, char* dataPointer)
{
	unsigned int firstEmptyBlock = -1;
	for (unsigned int blockIndex = 0; blockIndex < static_cast<unsigned int>(m_blockCount); blockIndex++)
	{
		Memory::EntityBlock entityBlock = m_entityBlocks[blockIndex];
		if (entityBlock.GetArchetype() == archetype)
		{
			Entity* entityArray = reinterpret_cast<Entity*>(entityBlock.GetDescriptor().m_data);
			for (unsigned int entityIndex = 0; entityIndex < static_cast<unsigned int>(entityBlock.GetMaxEntityCount()); entityIndex++)
			{
				Entity entity = entityArray[entityIndex];
				if (entity.GetId() != Entity::INVALID_ENTITY_ID)
				{
					entityBlock.InsertEntity(static_cast<int>(entityIndex), entity);
					if (dataPointer)
					{
						entityBlock.InsertEntityData(static_cast<int>(entityIndex), dataPointer);
						m_entityLocations[entity.GetId()].Set(blockIndex, entityIndex);
					}
				}
			}
		}
		else if (firstEmptyBlock == -1 && entityBlock.GetDescriptor().m_id == Memory::MemoryBlockDescriptor::INVALID_ID)
		{
			firstEmptyBlock = blockIndex;
		}
	}

	if (firstEmptyBlock > -1)
	{
		Memory::EntityBlock newBlock = m_entityBlocks[firstEmptyBlock];
		newBlock.Assign(m_allocator.Allocate(), archetype);
		newBlock.InsertEntity(0, entity);
		if (dataPointer)
		{
			newBlock.InsertEntityData(0, dataPointer);
			m_entityLocations[entity.GetId()].Set(firstEmptyBlock, 0);
		}
	}
	else
	{
		// TODO: Create a new block, increase the entity block array, add entity, when BlockAllocator supports dynamic block counts
	}
}
