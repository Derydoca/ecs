#pragma once
#include "BlockAllocator.h"
#include "Entity.h"
#include <array>
#include "TypeIdentifier.h"

namespace ECS
{

	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		Entity CreateEntity();

		template<typename ComponentType>
		void AddComponentData(Entity& entity, ComponentType data);

		template<typename ComponentType>
		void AddSharedComponentData(Entity& entity, ComponentType data);

	private:
		ECS::Memory::BlockAllocator m_allocator;
	};

}