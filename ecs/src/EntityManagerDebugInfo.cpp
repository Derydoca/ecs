#include "pch.h"
#include "EntityManagerDebugInfo.h"

namespace ECS
{
	EntityManagerDebugInfo::EntityManagerDebugInfo(const EntityManager* entityManager) :
		m_entityManager(entityManager)
	{}

	const size_t EntityManagerDebugInfo::GetEntityCount() const
	{
		size_t counter = 0;
		for (size_t i = 0; i < m_entityManager->m_nextEntityId; i++)
		{
			if (m_entityManager->m_entityLocations[i] != EntityLocation::INVALID_LOCATION)
			{
				counter++;
			}
		}
		return counter;
	}

	const size_t EntityManagerDebugInfo::GetNumberOfBlocksInUse() const
	{
		size_t count = 0;
		for (size_t i = 0; i < m_entityManager->m_blockCount; i++)
		{
			if (m_entityManager->m_entityBlocks[i].GetMaxEntityCount() != 0)
			{
				count++;
			}
		}
		return count;
	}

	const EntityArchetype EntityManagerDebugInfo::GetEntityBlockArchetype(const Entity entity) const
	{
		EntityLocation location = m_entityManager->m_entityLocations[entity.GetId()];
		return m_entityManager->m_entityBlocks[location.GetBlockIndex()].GetArchetype();
	}
}