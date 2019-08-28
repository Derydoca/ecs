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

		void CreateEntity(Entity& entity);
		void CreateEntity(Entity& entity, const EntityArchetype& archetype);
		void CreateEntityWithData(Entity& entity, const EntityArchetype archetype, char* dataPointer);
		void AddComponentData(Entity entity, TID tid);

		char* GetEntityDataPointer(const Entity entity, const TID componentTypeId);
		void SetEntityData(const Entity entity, const TID tid, char* componentData);

		template<typename ComponentType>
		ComponentType* GetEntityData(const Entity entity)
		{
			char* data = GetEntityDataPointer(entity, tid<ComponentType>());
			return reinterpret_cast<ComponentType*>(data);
		}

		template<typename ComponentType>
		void SetEntityData(const Entity entity, ComponentType& componentData)
		{
			SetEntityData(entity, tid<ComponentType>(), reinterpret_cast<char*>(&componentData));
		}

		void DeleteEntity(Entity& entity);

		void ReleaseEmptyBlocks();

		// Function Signature
		// void MyTransformationSystem(EntityManager manager, const Entity entity, Position2D& position);
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
							//... do the transformations here
							transformFunction(this, entity, reinterpret_cast<char*>(&componentPtr01[entityIndex]));
						}
					}
				}
			}
		}
	private:
		void InsertEntityDataInFirstOpenSlot(const Entity entity, const EntityArchetype archetype, char* dataPointer);
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