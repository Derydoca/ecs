#pragma once
#include "BlockAllocator.h"
#include "Entity.h"
#include <array>
#include "TypeIdentifier.h"

namespace ECS
{
	template<typename ... ComponentTypes>
	struct EntityArchetype
	{
	public:
		TypeIdentifier[16]& GetComponentTypes() const { return m_componentTypes; }
		int GetChunkCapacity() const { return m_chunkCapacity; }

	private:
		ComponentTypeIdentifier[16] m_componentTypes;
		int m_chunkCapacity;
	};

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