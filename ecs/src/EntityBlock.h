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

			const EntityArchetype& GetArchetype() const { return m_archetype; }
			char* GetComponentPointer(const TID& componentTypeId, int index = 0) const;
			const MemoryBlockDescriptor& GetDescriptor() const { return m_blockDescriptor; }
			const Entity GetEntity(const int entityIndex) const;
			Entity* GetEntityPointer() const { return reinterpret_cast<Entity*>(m_blockDescriptor.m_data); }
			const size_t GetMaxEntityCount() const { return m_maxEntityCount; }

			void Assign(const MemoryBlockDescriptor& blockDescriptor, const EntityArchetype& archetype);
			void DeleteEntity(const int entityIndex);
			void InsertEntity(const int entityIndex, const Entity entity);
			void Release();

			template<typename ComponentType>
			ComponentType* GetComponentPointer(int index = 0) const
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
