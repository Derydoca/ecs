#pragma once

namespace ECS
{
	namespace Memory
	{

		class MemoryFreeList
		{
		public:
			MemoryFreeList(size_t size);
			~MemoryFreeList();

			void Push(int id);
			int Pop();

		private:
			size_t m_size;
			unsigned int* m_stack;
			unsigned int m_stackHead;
		};

		struct MemoryBlockDescriptor
		{
			static const int INVALID_ID = 0;

			int m_id;
			size_t m_blockSize;
			char* m_data;

			MemoryBlockDescriptor() :
				m_id(INVALID_ID),
				m_blockSize(-1),
				m_data(0)
			{}

			MemoryBlockDescriptor(int id, size_t blockSize, char* data) :
				m_id(id),
				m_blockSize(blockSize),
				m_data(data)
			{}

			bool operator==(const MemoryBlockDescriptor& rhs) const
			{
				return m_data == rhs.m_data;
			}

			bool operator!=(const MemoryBlockDescriptor& rhs) const
			{
				return !operator==(rhs);
			}
		};

		class BlockAllocator
		{
		public:

			BlockAllocator(size_t size, size_t count);
			~BlockAllocator();

			MemoryBlockDescriptor Allocate();
			void Free(MemoryBlockDescriptor& memoryBlock);

		private:
			size_t m_blockSize;
			size_t m_count;
			char* m_data;
			MemoryFreeList m_freeList;
		};

	}
}

// Entity block
//===============
// Template Key
// Count
// Component 1
// Component 1
// Component 1
// Component 2
// Component 2
// Component 2
// ...