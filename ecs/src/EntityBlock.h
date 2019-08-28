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
			void Assign(MemoryBlockDescriptor blockDescriptor, EntityArchetype archetype);
			void Release();
			char* GetComponentPointer(TID componentTypeId, int index = 0);
			Entity* GetEntityPointer() { return reinterpret_cast<Entity*>(m_blockDescriptor.m_data); }

			template<typename ComponentType>
			ComponentType* GetComponentPointer(int index = 0)
			{
				return reinterpret_cast<ComponentType*>(GetComponentPointer(tid<ComponentType>()));
			}
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
