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
			EntityBlock(MemoryBlockDescriptor blockDescriptor, EntityArchetype archetype);
			const MemoryBlockDescriptor& GetDescriptor() { return m_blockDescriptor; }
			const EntityArchetype& GetArchetype() { return m_archetype; }
			const size_t GetMaxEntityCount() const { return m_maxEntityCount; }
			void InsertEntityData(const int entityIndex, const char* data);
			void DeleteEntityData(const int entityIndex);
			char* GetEntityMemoryAddress(const int entityIndex);
		private:
			MemoryBlockDescriptor m_blockDescriptor;
			EntityArchetype m_archetype;
			size_t m_maxEntityCount;
		};

	}
}
