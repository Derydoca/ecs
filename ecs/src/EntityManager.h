#pragma once
#include "BlockAllocator.h"
#include "Entity.h"
#include "TypeIdentifier.h"

namespace ECS
{

	class EntityManager
	{
	public:
		EntityManager(const size_t blockSize = 16 * 1024, const size_t blockCount = 16);
		~EntityManager();

		void CreateEntity(Entity& entity);

		template<typename ComponentType>
		void AddComponentData(Entity& entity, ComponentType data);

		template<typename ComponentType>
		void AddSharedComponentData(Entity& entity, ComponentType data);

	private:
		ECS::Memory::BlockAllocator m_allocator;
		int m_nextEntityId = 1;
	};

}