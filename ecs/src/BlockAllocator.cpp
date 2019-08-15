#include "pch.h"
#include "BlockAllocator.h"
#include <memory>
#include <cassert>

namespace ECS
{
	namespace Memory
	{

		BlockAllocator::BlockAllocator(size_t blockSize, size_t count) :
			m_blockSize(blockSize),
			m_count(count),
			m_freeList(count)
		{
			size_t allocSize = m_blockSize * m_count;
			m_data = reinterpret_cast<char*>(malloc(allocSize));
			memset(m_data, 0, allocSize);
		}

		BlockAllocator::~BlockAllocator()
		{
			free(m_data);
		}

		MemoryBlockDescriptor BlockAllocator::Allocate()
		{
			unsigned int id = m_freeList.Pop();
			void* data = 0;
			if (id >= 0)
			{
				data = &m_data + (m_blockSize * id);
			}
			return MemoryBlockDescriptor(id, m_blockSize, m_data);
		}

		void BlockAllocator::Free(MemoryBlockDescriptor& memoryBlock)
		{
			m_freeList.Push(memoryBlock.m_id);

			memoryBlock.m_blockSize = 0;
			memoryBlock.m_data = nullptr;
			memoryBlock.m_id = -1;
		}

		MemoryFreeList::MemoryFreeList(size_t size) :
			m_size(size),
			m_stackHead(0)
		{
			m_stack = static_cast<unsigned int*>(malloc(size * sizeof(unsigned int)));
			for (size_t i = 0; i < m_size; i++)
			{
				m_stack[i] = static_cast<int>(i);
			}
		}

		MemoryFreeList::~MemoryFreeList()
		{
			free(m_stack);
		}

		void MemoryFreeList::Push(int id)
		{
			m_stack[--m_stackHead] = id;
			assert(id >= 0);
		}

		int MemoryFreeList::Pop()
		{
			return m_stack[m_stackHead++];
		}

	}
}
