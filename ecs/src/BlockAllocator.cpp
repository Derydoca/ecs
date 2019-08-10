#include "pch.h"
#include "BlockAllocator.h"
#include <memory>

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
			m_data = malloc(allocSize);
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
			MemoryBlockDescriptor(id, m_data);
		}

		void BlockAllocator::Free(int blockId)
		{
			m_freeList.Push(blockId);
		}

		MemoryFreeList::MemoryFreeList(size_t size) :
			m_size(size),
			m_stackHead(0)
		{
			m_stack = static_cast<unsigned int*>(malloc(size * sizeof(unsigned int)));
			for (size_t i = 0; i < m_size; i++)
			{
				m_stack[i] = i;
			}
		}

		MemoryFreeList::~MemoryFreeList()
		{
			free(m_stack);
		}

		void MemoryFreeList::Push(int id)
		{
			m_stack[m_stackHead++] = id;
		}

		int MemoryFreeList::Pop()
		{
			if (m_stackHead < 1)
			{
				return -1;
			}

			return m_stack[m_stackHead--];
		}

	}
}
