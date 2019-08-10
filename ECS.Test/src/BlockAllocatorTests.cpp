#include "pch.h"
#include "BlockAllocator.h"

TEST(BlockAllocator, AllocatingBlock_Has_BlockSize) {
	ECS::Memory::BlockAllocator allocator(1024, 10);
	ECS::Memory::MemoryBlockDescriptor descriptor = allocator.Allocate();
	ASSERT_EQ(descriptor.m_blockSize, 1024);
}

TEST(BlockAllocator, AllocatingFirstBlock_Has_BlockIdOfZero) {
	ECS::Memory::BlockAllocator allocator(1024, 10);
	ECS::Memory::MemoryBlockDescriptor descriptor = allocator.Allocate();
	ASSERT_EQ(descriptor.m_id, 0);
}

TEST(BlockAllocator, AllocatingSecondBlock_Has_BlockIdOfOne) {
	ECS::Memory::BlockAllocator allocator(1024, 10);
	ECS::Memory::MemoryBlockDescriptor descriptor0 = allocator.Allocate();
	ECS::Memory::MemoryBlockDescriptor descriptor1 = allocator.Allocate();
	ASSERT_EQ(descriptor1.m_id, 1);
}