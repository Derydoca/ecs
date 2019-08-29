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

		char* EntityBlock::GetComponentPointer(const TID& componentTypeId, int index) const
		{
			assert(m_blockDescriptor.m_data != nullptr);
			assert(index < m_maxEntityCount);

			// Start the offset with the header size
			size_t offset = m_maxEntityCount * sizeof(Entity);

			// Increment the offset to point at the start of the component stream
			offset += m_archetype.GetTypeOffset(componentTypeId) * m_maxEntityCount;

			// Jump to the specific component by index
			offset += componentTypeId.GetSize() * index;
			
			// Return the pointer to the calculated position
			return m_blockDescriptor.m_data + offset;
		}

		const Entity EntityBlock::GetEntity(const int entityIndex) const
		{
			assert(entityIndex < m_maxEntityCount);
			return reinterpret_cast<Entity*>(m_blockDescriptor.m_data)[entityIndex];
		}

		void EntityBlock::Assign(const MemoryBlockDescriptor& blockDescriptor, const EntityArchetype& archetype)
		{
			m_blockDescriptor = blockDescriptor;
			m_archetype = archetype;

			Initialize();
		}

		void EntityBlock::DeleteEntity(const int entityIndex)
		{
			assert(entityIndex < m_maxEntityCount);

			// Delete the entity entry in the header
			GetEntityPointer()[entityIndex] = Entity::INVALID_ENTITY_ID;

#if ECS_DEBUG
			// Clear all memory of components associated with the entity
			for (size_t i = 0; i < MAX_TYPE_IDENTIFIER_COUNT; i++)
			{
				TID tid = m_archetype.GetType(i);

				// Break out early at the first instance of a null type ID
				if (tid == NULL_TID)
				{
					break;
				}

				// Reset the memory for this component
				char* componentPtr = GetComponentPointer(tid, entityIndex);
				memset(componentPtr, 0, tid.GetSize());
			}
#endif
		}

		void EntityBlock::InsertEntity(const int entityIndex, const Entity entity)
		{
			assert(entityIndex < m_maxEntityCount);
			reinterpret_cast<Entity*>(m_blockDescriptor.m_data)[entityIndex] = entity;
		}

		void EntityBlock::Release()
		{
			m_archetype = EntityArchetype::Empty;
			m_maxEntityCount = 0;
		}

		void EntityBlock::Initialize()
		{
			size_t blockSize = m_blockDescriptor.m_blockSize;
			size_t entitySize = m_archetype.GetEntitySize();
			m_maxEntityCount = blockSize / (entitySize + sizeof(Entity));

			// Format the data in the block if one is provided
			if (m_blockDescriptor.m_data)
			{
				size_t headerSize = sizeof(Entity) * m_maxEntityCount;
				// Set all entities to an invalid ID
				memset(m_blockDescriptor.m_data, Entity::INVALID_ENTITY_ID, headerSize);
#if ECS_DEBUG
				// Set all remaining block data to zeros
				memset(m_blockDescriptor.m_data + headerSize, 0, m_blockDescriptor.m_blockSize - headerSize);
#endif
			}
		}

	}
}
