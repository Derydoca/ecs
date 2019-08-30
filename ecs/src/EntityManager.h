#pragma once
#include "BlockAllocator.h"
#include "Entity.h"
#include "TypeIdentifier.h"
#include "EntityArchetype.h"
#include "EntityBlock.h"
#include "EntityLocation.h"

namespace ECS
{

	class EntityManager
	{
	public:
		EntityManager(const size_t blockSize = 16 * 1024, const size_t blockCount = 16);
		~EntityManager();

		void CreateEntity(Entity& entity, const EntityArchetype& archetype = EntityArchetype::Empty);
		void DeleteEntity(Entity& entity);
		void AddComponentData(Entity entity, TID tid);

		char* GetEntityDataPointer(const Entity entity, const TID componentTypeId) const;
		void ReleaseEmptyBlocks();
		void SetEntityData(const Entity entity, const TID tid, char* componentData);

		template<typename ComponentType>
		ComponentType* GetEntityData(const Entity entity) const
		{
			char* data = GetEntityDataPointer(entity, tid<ComponentType>());
			return reinterpret_cast<ComponentType*>(data);
		}

		template<typename ComponentType>
		void SetEntityData(const Entity entity, ComponentType& componentData)
		{
			SetEntityData(entity, tid<ComponentType>(), reinterpret_cast<char*>(&componentData));
		}

		typedef void(*TransformFunction00)(EntityManager*, const Entity, char*);
		template<typename ComponentType00>
		void TransformData(TransformFunction00 transformFunction)
		{
			EntityArchetype targetArchetype = EntityArchetype(tid<ComponentType00>());

			Entity* entityPtr = nullptr;
			ComponentType00* componentPtr01 = nullptr;

			for (size_t blockIndex = 0; blockIndex < m_blockCount; blockIndex++)
			{
				Memory::EntityBlock block = m_entityBlocks[blockIndex];
				EntityArchetype blockArchetype = block.GetArchetype();
				if (targetArchetype.ArchetypeContainsAllMyTypes(blockArchetype))
				{
					entityPtr = block.GetEntityPointer();
					componentPtr01 = block.GetComponentPointer<ComponentType00>();
					for (size_t entityIndex = 0; entityIndex < block.GetMaxEntityCount(); entityIndex++)
					{
						Entity entity = entityPtr[entityIndex];
						if (entityPtr[entityIndex].m_id != Entity::INVALID_ENTITY_ID)
						{
							transformFunction(this, entity, reinterpret_cast<char*>(&componentPtr01[entityIndex]));
						}
					}
				}
			}
		}

	private:
		Memory::BlockAllocator m_allocator;
		size_t m_blockCount;
		int m_nextEntityId;
		Memory::EntityBlock* m_entityBlocks;
		EntityLocation* m_entityLocations;
		size_t m_maxEntities;

		friend class EntityManagerDebugInfo;
	};

}