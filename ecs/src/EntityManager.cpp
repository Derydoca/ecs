#include "pch.h"
#include "EntityManager.h"
#include <cassert>

ECS::EntityManager::EntityManager(const size_t blockSize, const size_t blockCount) :
	m_allocator(blockSize, blockCount),
	m_nextEntityId(0),
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
	CreateEntityWithData(entity, EntityArchetype::Empty, nullptr);
}

void ECS::EntityManager::CreateEntity(Entity& entity, const EntityArchetype& archetype)
{
	CreateEntityWithData(entity, archetype, nullptr);
}

void ECS::EntityManager::CreateEntityWithData(Entity& entity, const EntityArchetype archetype, char* dataPointer)
{
	assert(entity.m_id < 0);
	entity.m_id = m_nextEntityId++;
	InsertEntityInFirstOpenSlot(entity, archetype);
}

void ECS::EntityManager::AddComponentData(Entity entity, TID tid)
{
	EntityLocation sourceLocation = m_entityLocations[entity.GetId()];
	unsigned int sourceEntityIndex = sourceLocation.GetEntityIndex();
	Memory::EntityBlock* sourceEntityBlock = &m_entityBlocks[sourceLocation.GetBlockIndex()];
	EntityArchetype sourceArchetype = sourceEntityBlock->GetArchetype();
	EntityArchetype destinationArchetype = EntityArchetype::CreateArchetypeWithNewType(sourceArchetype, tid);

	int nextOpenBlockIndex = -1;
	unsigned int destinationBlockIndex = 0;
	unsigned int destinationEntityIndex = 0;
	bool destinationLocationFound = false;
	for (unsigned int blockIndex = 0; blockIndex < static_cast<unsigned int>(m_blockCount); blockIndex++)
	{
		Memory::EntityBlock* entityBlock = &m_entityBlocks[blockIndex];
		if (entityBlock->GetArchetype().Equals(destinationArchetype, ECS::EntityArchetypeComparisonFlags::IgnoreOrder))
		{
			Entity* entityArray = reinterpret_cast<Entity*>(entityBlock->GetDescriptor().m_data);
			for (unsigned int entityIndex = 0; entityIndex < static_cast<unsigned int>(entityBlock->GetMaxEntityCount()); entityIndex++)
			{
				Entity entity = entityArray[entityIndex];
				if (entity.m_id == Entity::INVALID_ENTITY_ID)
				{
					destinationBlockIndex = blockIndex;
					destinationEntityIndex = entityIndex;
					destinationLocationFound = true;
				}
			}
		}
		else if (nextOpenBlockIndex < 0 && entityBlock->GetArchetype() == EntityArchetype::Empty)
		{
			nextOpenBlockIndex = blockIndex;
		}
	}

	if (!destinationLocationFound && nextOpenBlockIndex >= 0)
	{
		Memory::EntityBlock* entityBlock = &m_entityBlocks[nextOpenBlockIndex];
		entityBlock->Assign(m_allocator.Allocate(), destinationArchetype);
		destinationBlockIndex = nextOpenBlockIndex;
		destinationEntityIndex = 0;
		destinationLocationFound = true;
	}

	assert(destinationLocationFound);

	Memory::EntityBlock* destinationBlock = &m_entityBlocks[destinationBlockIndex];
	for (size_t i = 0; i < MAX_TYPE_IDENTIFIER_COUNT; i++)
	{
		TID tid = sourceArchetype.GetType(i);
		if (tid == NULL_TID)
		{
			continue;
		}

		assert(sourceArchetype.GetTypeOffset(tid) >= 0);

		char* sourceComponentPtr = sourceEntityBlock->GetComponentPointer(tid, sourceEntityIndex);
		char* destinationComponentPtr = destinationBlock->GetComponentPointer(tid, destinationEntityIndex);

		memcpy(destinationComponentPtr, sourceComponentPtr, tid.GetSize());
	}

	m_entityLocations[entity.GetId()].Set(destinationBlockIndex, destinationEntityIndex);
	sourceEntityBlock->DeleteEntity(sourceLocation.GetEntityIndex());
	destinationBlock->InsertEntity(destinationEntityIndex, entity);
}

char* ECS::EntityManager::GetEntityDataPointer(Entity entity, TID componentTypeId)
{
	EntityLocation location = m_entityLocations[entity];
	Memory::EntityBlock entityBlock = m_entityBlocks[location.GetBlockIndex()];
	char* componentStartPtr = entityBlock.GetComponentPointer(componentTypeId);
	componentStartPtr += componentTypeId.GetSize() * location.GetEntityIndex();
	return componentStartPtr;
}

void ECS::EntityManager::SetEntityData(Entity entity, TID componentTypeId, char* componentData)
{
	EntityLocation location = m_entityLocations[entity];
	Memory::EntityBlock entityBlock = m_entityBlocks[location.GetBlockIndex()];
	char* componentStartPtr = entityBlock.GetComponentPointer(componentTypeId, location.GetEntityIndex());
	memcpy(componentStartPtr, componentData, componentTypeId.GetSize());
}

void ECS::EntityManager::DeleteEntity(Entity& entity)
{
	assert(entity.GetId() != Entity::INVALID_ENTITY_ID);
	EntityLocation location = m_entityLocations[entity.GetId()];
	unsigned int blockIndex = location.GetBlockIndex();
	unsigned int entityIndex = location.GetEntityIndex();
	m_entityBlocks[blockIndex].DeleteEntity(entityIndex);
	m_entityLocations[entity.GetId()].ResetToInvalid();
}

void ECS::EntityManager::ReleaseEmptyBlocks()
{
	for (unsigned int blockIndex = 0; blockIndex < static_cast<unsigned int>(m_blockCount); blockIndex++)
	{
		Memory::EntityBlock* entityBlock = &m_entityBlocks[blockIndex];
		if (entityBlock->GetMaxEntityCount() != 0 && entityBlock->GetArchetype() != EntityArchetype::Empty)
		{
			bool blockIsEmpty = true;
			Entity* entityArray = reinterpret_cast<Entity*>(entityBlock->GetDescriptor().m_data);
			for (unsigned int i = 0; i < static_cast<unsigned int>(entityBlock->GetMaxEntityCount()); i++)
			{
				if (entityArray[i].GetId() != Entity::INVALID_ENTITY_ID)
				{
					blockIsEmpty = false;
					break;
				}
			}
			if (blockIsEmpty)
			{
				m_allocator.Free(entityBlock->m_blockDescriptor);
				entityBlock->Release();
			}
		}
	}
}

void ECS::EntityManager::InsertEntityInFirstOpenSlot(const Entity entity, const EntityArchetype archetype)
{
	unsigned int firstEmptyBlock = -1;
	for (unsigned int blockIndex = 0; blockIndex < static_cast<unsigned int>(m_blockCount); blockIndex++)
	{
		Memory::EntityBlock* entityBlock = &m_entityBlocks[blockIndex];
		if (entityBlock->GetMaxEntityCount() != 0 && entityBlock->GetArchetype() == archetype)
		{
			Entity* entityArray = reinterpret_cast<Entity*>(entityBlock->GetDescriptor().m_data);
			for (unsigned int entityIndex = 0; entityIndex < static_cast<unsigned int>(entityBlock->GetMaxEntityCount()); entityIndex++)
			{
				Entity blockEntity = entityArray[entityIndex];
				if (blockEntity.GetId() == Entity::INVALID_ENTITY_ID)
				{
					entityBlock->InsertEntity(static_cast<int>(entityIndex), entity);
					m_entityLocations[entity.GetId()].Set(blockIndex, entityIndex);
					return;
				}
			}
		}
		else if (firstEmptyBlock == -1 && entityBlock->GetDescriptor().m_id == Memory::MemoryBlockDescriptor::INVALID_ID)
		{
			firstEmptyBlock = blockIndex;
		}
	}

	if (firstEmptyBlock != -1)
	{
		Memory::EntityBlock* newBlock = &m_entityBlocks[firstEmptyBlock];
		Memory::MemoryBlockDescriptor memoryBlockDescriptor = m_allocator.Allocate();
		newBlock->Assign(memoryBlockDescriptor, archetype);
		newBlock->InsertEntity(0, entity);
		m_entityLocations[entity.GetId()].Set(firstEmptyBlock, 0);
	}
	else
	{
		// TODO: Create a new block, increase the entity block array, add entity, when BlockAllocator supports dynamic block counts
	}
}
