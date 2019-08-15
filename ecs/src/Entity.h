#pragma once

namespace ECS
{
	struct Entity
	{
	public:
		Entity(int id);

		const int GetId() { return m_id; }
	private:
		int m_id;
	};
}