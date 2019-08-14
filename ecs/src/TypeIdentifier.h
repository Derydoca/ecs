#pragma once

namespace ECS
{

	using TypeIdentifier = intptr_t;

	template<typename type>
	TypeIdentifier GetTypeIdentifier()
	{
		static type m_instance;
		return reinterpret_cast<intptr_t>(&m_instance);
	}

}
