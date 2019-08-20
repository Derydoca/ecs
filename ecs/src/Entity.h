#pragma once

namespace ECS
{
	struct Entity
	{
	public:
		static const int INVALID_ENTITY_ID = 0;

		Entity(int id = INVALID_ENTITY_ID);

		const int GetId() const
		{
			return (m_id & 0xFFFF);
		}

		bool operator==(const Entity& rhs) const
		{
			return m_id == rhs.m_id;
		}

		bool operator!=(const Entity& rhs) const
		{
			return m_id != rhs.m_id;
		}
	private:
		int m_id;

		friend class EntityManager;
	};
}