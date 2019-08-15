#pragma once
#include <cstdint>

namespace ECS
{

	struct TID
	{
	public:
		TID(uintptr_t id, size_t size) :
			m_id(id),
			m_size(size)
		{}

		uintptr_t GetId() const { return m_id; }
		size_t GetSize() const { return m_size; }

		bool operator==(const TID& rhs) const
		{
			return m_id == rhs.m_id;
		}

		bool operator!=(const TID& rhs) const
		{
			return m_id != rhs.m_id;
		}
	private:
		uintptr_t m_id;
		size_t m_size;
	};

	const static TID NULL_TID = { 0, 0 };

	template<typename type>
	TID tid()
	{
		static type m_instance;
		return TID(reinterpret_cast<intptr_t>(&m_instance), sizeof(type));
	}

}
