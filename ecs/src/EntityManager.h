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
		void SetEntityData(const Entity entity, const TID tid, char* componentData, const size_t componentDataSize);

		template<typename ComponentType>
		ComponentType* GetEntityData(const Entity entity)
		{
			char* data = GetEntityDataPointer(entity, tid<ComponentType>());
			return reinterpret_cast<ComponentType*>(data);
		}

		template<typename ComponentType>
		void SetEntityData(const Entity entity, ComponentType& componentData)
		{
			SetEntityData(entity, tid<ComponentType>(), reinterpret_cast<char*>(&componentData), sizeof(ComponentType));
		}

		void DeleteEntity(Entity& entity);

		void ReleaseEmptyBlocks();
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