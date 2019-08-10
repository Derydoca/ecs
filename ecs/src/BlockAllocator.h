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
			const static int BlockSize = 1024 * 64;

			void* m_data;
			int m_id;

			MemoryBlockDescriptor(int id, void* data) :
				m_id(id),
				m_data(data)
			{
			}
		};

		class BlockAllocator
		{
		public:

			BlockAllocator(size_t size, size_t count);
			~BlockAllocator();

			MemoryBlockDescriptor Allocate();
			void Free(int blockId);

		private:
			size_t m_blockSize;
			size_t m_count;
			void* m_data;
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