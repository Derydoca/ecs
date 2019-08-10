#include "pch.h"
#include "BlockAllocator.h"

TEST(MemoryFreeList, TheFirstPop_Equals_Zero)
{
	ECS::Memory::MemoryFreeList freeList(5);
	int poppedValue = freeList.Pop();
	ASSERT_EQ(poppedValue, 0);
}

TEST(MemoryFreeList, TheSecondPop_Equals_One)
{
	ECS::Memory::MemoryFreeList freeList(5);
	freeList.Pop();
	int poppedValue = freeList.Pop();
	ASSERT_EQ(poppedValue, 1);
}

TEST(MemoryFreeList, TheThirdPop_Equals_Two)
{
	ECS::Memory::MemoryFreeList freeList(5);
	freeList.Pop();
	freeList.Pop();
	int poppedValue = freeList.Pop();
	ASSERT_EQ(poppedValue, 2);
}