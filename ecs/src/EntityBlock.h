#pragma once
#include "BlockAllocator.h"
#include "EntityArchetype.h"

namespace ECS
{
	namespace Memory
	{

		class EntityBlock
		{
		public:
			EntityBlock();
			EntityBlock(MemoryBlockDescriptor blockDescriptor, EntityArchetype archetype);
			const MemoryBlockDescriptor& GetDescriptor() { return m_blockDescriptor; }
			const EntityArchetype& GetArchetype() { return m_archetype; }
			const size_t GetMaxEntityCount() const { return m_maxEntityCount; }
			void InsertEntity(const int entityIndex, const Entity entity);
			void InsertEntityData(const int entityIndex, const char* data);
			void DeleteEntity(const int entityIndex);
			const Entity GetEntity(const int entityIndex) const;
			char* GetEntityMemoryAddress(const int entityIndex);
			void Assign(MemoryBlockDescriptor blockDescriptor, EntityArchetype archetype);
			void InsertEntityCopy(unsigned int entityIndex, Entity entity, EntityArchetype sourceArchetype, char* sourceEntityData);
			void Release();
		private:
			void Initialize();
		private:
			MemoryBlockDescriptor m_blockDescriptor;
			EntityArchetype m_archetype;
			size_t m_maxEntityCount;

			friend EntityManager;
		};

	}
}
