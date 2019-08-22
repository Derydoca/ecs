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
			memcpy(GetEntityMemoryAddress(entityIndex), data, m_archetype.GetEntitySize());
		}

		void EntityBlock::DeleteEntity(const int entityIndex)
		{
			assert(entityIndex < m_maxEntityCount);
			reinterpret_cast<Entity*>(m_blockDescriptor.m_data)[entityIndex] = Entity::INVALID_ENTITY_ID;
			memset(GetEntityMemoryAddress(entityIndex), 0, m_archetype.GetEntitySize());
		}

		const Entity EntityBlock::GetEntity(const int entityIndex) const
		{
			assert(entityIndex < m_maxEntityCount);
			return reinterpret_cast<Entity*>(m_blockDescriptor.m_data)[entityIndex];
		}

		char* EntityBlock::GetEntityMemoryAddress(const int entityIndex)
		{
			assert(entityIndex < m_maxEntityCount);
			return &m_blockDescriptor.m_data[sizeof(Entity) * m_maxEntityCount + entityIndex * m_archetype.GetEntitySize()];
		}

		void EntityBlock::Assign(MemoryBlockDescriptor blockDescriptor, EntityArchetype archetype)
		{
			m_blockDescriptor = blockDescriptor;
			m_archetype = archetype;
			Initialize();
		}

		void EntityBlock::InsertEntityCopy(unsigned int entityIndex, Entity entity, EntityArchetype sourceArchetype, char* sourceEntityData)
		{
			InsertEntity(entityIndex, entity);
			InsertEntityData(entityIndex, sourceEntityData);
		}

		void EntityBlock::Release()
		{
			m_archetype = EntityArchetype::Empty;
			// TODO: Evaluate if this should be a pointer to a block descriptor instead
			//m_blockDescriptor = nullptr;
			m_maxEntityCount = 0;
		}

		void EntityBlock::Initialize()
		{
			size_t blockSize = m_blockDescriptor.m_blockSize;
			size_t entitySize = m_archetype.GetEntitySize();
			if (entitySize == 0)
			{
				m_maxEntityCount = blockSize / sizeof(Entity);
			}
			else
			{
				size_t headerOffset = blockSize / entitySize;
				m_maxEntityCount = (blockSize - headerOffset) / entitySize;
			}

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
