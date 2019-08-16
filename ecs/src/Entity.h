#pragma once

namespace ECS
{
	struct Entity
	{
	public:
		static const int INVALID_ENTITY_ID = 0;

		Entity(int id = INVALID_ENTITY_ID);

		const int GetId() { return m_id; }
	private:
		int m_id;

		friend class EntityManager;
	};
}