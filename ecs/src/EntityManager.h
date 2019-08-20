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
		void CreateEntityWithData(Entity& entity, const EntityArchetype archetype, char* dataPointer);
		void AddComponentData(Entity entity, TID tid);
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