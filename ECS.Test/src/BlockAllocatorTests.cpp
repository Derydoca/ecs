#include "pch.h"
#include "BlockAllocator.h"

TEST(BlockAllocator, AllocatingBlock_Has_BlockSize) {
	ECS::Memory::BlockAllocator allocator(1024, 10);
	ECS::Memory::MemoryBlockDescriptor descriptor = allocator.Allocate();
	ASSERT_EQ(descriptor.m_blockSize, 1024);
	allocator.Free(descriptor);
}

TEST(BlockAllocator, AllocatingFirstBlock_Has_BlockIdOfZero) {
	ECS::Memory::BlockAllocator allocator(1024, 10);
	ECS::Memory::MemoryBlockDescriptor descriptor = allocator.Allocate();
	ASSERT_EQ(descriptor.m_id, 0);
	allocator.Free(descriptor);
}

TEST(BlockAllocator, AllocatingSecondBlock_Has_BlockIdOfOne) {
	ECS::Memory::BlockAllocator allocator(1024, 10);
	ECS::Memory::MemoryBlockDescriptor descriptor0 = allocator.Allocate();
	ECS::Memory::MemoryBlockDescriptor descriptor1 = allocator.Allocate();
	ASSERT_EQ(descriptor1.m_id, 1);
	allocator.Free(descriptor0);
	allocator.Free(descriptor1);
}

TEST(BlockAllocator, DeallocatingABlock_Sets_BlockSizeToZero) {
	ECS::Memory::BlockAllocator allocator(1024, 10);
	ECS::Memory::MemoryBlockDescriptor descriptor = allocator.Allocate();
	ASSERT_TRUE(descriptor.m_blockSize > 0);
	allocator.Free(descriptor);
	ASSERT_EQ(descriptor.m_blockSize, 0);
}

TEST(BlockAllocator, DeallocatingABlock_Sets_DataToNullPtr) {
	ECS::Memory::BlockAllocator allocator(1024, 10);
	ECS::Memory::MemoryBlockDescriptor descriptor = allocator.Allocate();
	ASSERT_TRUE(descriptor.m_data != nullptr);
	allocator.Free(descriptor);
	ASSERT_EQ(descriptor.m_data, nullptr);
}

TEST(BlockAllocator, DeallocatingABlock_Sets_IdToInvalidValue) {
	ECS::Memory::BlockAllocator allocator(1024, 10);
	ECS::Memory::MemoryBlockDescriptor descriptor = allocator.Allocate();
	ASSERT_TRUE(descriptor.m_id >= 0);
	allocator.Free(descriptor);
	ASSERT_EQ(descriptor.m_id, -1);
}