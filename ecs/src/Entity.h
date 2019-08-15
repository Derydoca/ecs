#pragma once

namespace ECS
{
	struct Entity
	{
	public:
		Entity(int id = -1);

		const int GetId() { return m_id; }
	private:
		int m_id;

		friend class EntityManager;
	};
}