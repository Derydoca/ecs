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
			return m_entityManager->m_nextEntityId - 1;
		}
	private:
		const EntityManager* m_entityManager;
	};
}