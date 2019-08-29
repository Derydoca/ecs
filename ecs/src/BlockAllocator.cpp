#include "pch.h"
#include "BlockAllocator.h"
#include <memory>
#include <cassert>

namespace ECS
{
	namespace Memory
	{

		MemoryBlockFreeList::MemoryBlockFreeList(size_t size) :
			m_size(size),
			m_stack(nullptr),
			m_stackHead(0)
		{
			// Create the stack on the heap
			m_stack = new unsigned int[size];

			// Prime the stack with incrementing integers
			for (size_t i = 0; i < m_size; i++)
			{
				m_stack[i] = static_cast<int>(i);
			}
		}

		MemoryBlockFreeList::~MemoryBlockFreeList()
		{
			delete[](m_stack);
		}

		void MemoryBlockFreeList::Push(int id)
		{
			m_stack[--m_stackHead] = id;
			assert(id >= 0);
		}

		int MemoryBlockFreeList::Pop()
		{
			return m_stack[m_stackHead++];
			assert(m_stackHead <= m_size);
		}

		void MemoryBlockDescriptor::Invalidate()
		{
			m_blockSize = 0;
			m_data = nullptr;
			m_id = -1;
		}

		BlockAllocator::BlockAllocator(size_t blockSize, size_t count) :
			m_blockSize(blockSize),
			m_count(count),
			m_data(nullptr),
			m_freeList(count)
		{
			// Allocate a block of memory large enough to hold all entity blocks
			size_t allocSize = m_blockSize * m_count;
			m_data = reinterpret_cast<char*>(malloc(allocSize));

#if ECS_DEBUG
			// Clear all of the data in the blocks
			memset(m_data, 0, allocSize);
#endif
		}

		BlockAllocator::~BlockAllocator()
		{
			free(m_data);
		}

		MemoryBlockDescriptor BlockAllocator::Allocate()
		{
			// Pop a block ID off of the free list
			unsigned int id = m_freeList.Pop();

			// Get the block's data pointer
			char* blockData = 0;
			if (id >= 0)
			{
				blockData = m_data + (m_blockSize * id);
			}

			// Construct and return a block descriptor
			return MemoryBlockDescriptor(id, m_blockSize, blockData);
		}

		void BlockAllocator::Free(MemoryBlockDescriptor& memoryBlock)
		{
			// Push the block's ID back onto the free list
			m_freeList.Push(memoryBlock.m_id);

			// Clear the descriptor
			memoryBlock.Invalidate();
		}

	}
}
