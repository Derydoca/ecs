#pragma once
#include "EntityManager.h"

namespace ECS
{
	class EntityManagerDebugInfo
	{
	public:
		EntityManagerDebugInfo(const EntityManager* entityManager);

		const size_t GetEntityCount() const
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
	private:
		const EntityManager* m_entityManager;
	};
}