#pragma once
#include "BlockAllocator.h"
#include "Entity.h"
#include <array>

namespace ECS
{
	struct ComponentTypeIdentifier
	{
		int Id;

		/*template<struct ComponentType>
		static ComponentTypeIdentifier Get(ComponentType type)
		{

		}*/
	};

	template<typename ... ComponentTypes>
	struct EntityArchetype
	{
	public:
		ComponentTypeIdentifier[16]& GetComponentTypes() const { return m_componentTypes; }
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

		EntityArchetype CreateArchetype()

	private:
		ECS::Memory::BlockAllocator m_allocator;
	};

}