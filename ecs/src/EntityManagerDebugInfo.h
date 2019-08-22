#pragma once
#include "EntityManager.h"

namespace ECS
{
	class EntityManagerDebugInfo
	{
	public:
		EntityManagerDebugInfo(const EntityManager* entityManager);

		const size_t GetEntityCount() const;
		const size_t GetNumberOfBlocksInUse() const;

		const EntityArchetype GetEntityBlockArchetype(const Entity entity) const;
	private:
		const EntityManager* m_entityManager;
	};
}