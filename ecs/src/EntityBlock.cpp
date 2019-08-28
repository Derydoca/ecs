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

		void EntityBlock::InsertEntity(const int entityIndex, const Entity entity)
		{
			assert(entityIndex < m_maxEntityCount);
			reinterpret_cast<Entity*>(m_blockDescriptor.m_data)[entityIndex] = entity;
		}

		void EntityBlock::InsertEntityData(const int entityIndex, const char* data)
		{
			assert(entityIndex < m_maxEntityCount);
			for (size_t i = 0; i < MAX_TYPE_IDENTIFIER_COUNT; i++)
			{
				TID tid = m_archetype.GetType(i);
				size_t typeSize = tid.GetSize();
				size_t typeOffset = m_archetype.GetTypeOffset(tid);
				char* componentPtr = GetComponentPointer(tid) + typeSize * entityIndex;

				memcpy(componentPtr, data + typeOffset, typeSize);
			}
		}

		void EntityBlock::DeleteEntity(const int entityIndex)
		{
			assert(entityIndex < m_maxEntityCount);
			GetEntityPointer()[entityIndex] = Entity::INVALID_ENTITY_ID;
			for (size_t i = 0; i < MAX_TYPE_IDENTIFIER_COUNT; i++)
			{
				TID tid = m_archetype.GetType(i);
				size_t typeSize = tid.GetSize();
				size_t typeOffset = m_archetype.GetTypeOffset(tid);
				char* componentPtr = GetComponentPointer(tid) + typeSize * entityIndex;

				memset(componentPtr, 0, typeSize);
			}
		}

		const Entity EntityBlock::GetEntity(const int entityIndex) const
		{
			assert(entityIndex < m_maxEntityCount);
			return reinterpret_cast<Entity*>(m_blockDescriptor.m_data)[entityIndex];
		}

		void EntityBlock::Assign(MemoryBlockDescriptor blockDescriptor, EntityArchetype archetype)
		{
			m_blockDescriptor = blockDescriptor;
			m_archetype = archetype;
			Initialize();
		}

		void EntityBlock::Release()
		{
			m_archetype = EntityArchetype::Empty;
			// TODO: Evaluate if this should be a pointer to a block descriptor instead
			//m_blockDescriptor = nullptr;
			m_maxEntityCount = 0;
		}

		char* EntityBlock::GetComponentPointer(TID componentTypeId, int index)
		{
			assert(m_blockDescriptor.m_data != nullptr);
			assert(index < m_maxEntityCount);
			size_t offset = m_archetype.GetTypeOffset(componentTypeId);
			offset *= m_maxEntityCount;
			offset += m_maxEntityCount * sizeof(Entity);
			return m_blockDescriptor.m_data + offset + componentTypeId.GetSize() * index;
		}

		void EntityBlock::Initialize()
		{
			size_t blockSize = m_blockDescriptor.m_blockSize;
			size_t entitySize = m_archetype.GetEntitySize();
			m_maxEntityCount = blockSize / (entitySize + sizeof(Entity));

			// Set all entities to an invalid ID
			if (m_blockDescriptor.m_data)
			{
				size_t headerSize = sizeof(Entity) * m_maxEntityCount;
				memset(m_blockDescriptor.m_data, Entity::INVALID_ENTITY_ID, headerSize);
				memset(m_blockDescriptor.m_data + headerSize, 0, m_blockDescriptor.m_blockSize - headerSize);
			}
		}

	}
}
