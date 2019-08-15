#include "pch.h"
#include "EntityBlock.h"

TEST(EntityBlock, ReturnedDescriptor_Equals_ConstructorDescriptor) {
	ECS::EntityArchetype archetype(ECS::tid<int>(), ECS::tid<float>());
	size_t blockSize = 256;
	void* data = malloc(blockSize);
	ECS::Memory::MemoryBlockDescriptor descriptor(25, blockSize, data);
	ECS::Memory::EntityBlock entityBlock(descriptor, archetype);
	ASSERT_EQ(entityBlock.GetDescriptor(), descriptor);
	free(data);
}

TEST(EntityBlock, ReturnedArchetype_Equals_ConstructorArchetype) {
	ECS::EntityArchetype archetype(ECS::tid<int>(), ECS::tid<float>());
	size_t blockSize = 256;
	void* data = malloc(blockSize);
	ECS::Memory::MemoryBlockDescriptor descriptor(25, blockSize, data);
	ECS::Memory::EntityBlock entityBlock(descriptor, archetype);
	ASSERT_EQ(entityBlock.GetArchetype(), archetype);
	free(data);
}

TEST(EntityBlock, MaxEntityCount_Equals_ExpectedAmount) {
	ECS::EntityArchetype archetype(ECS::tid<int>(), ECS::tid<float>());
	size_t blockSize = archetype.GetEntitySize() * 25;
	void* data = malloc(blockSize);
	ECS::Memory::MemoryBlockDescriptor descriptor(25, blockSize, data);
	ECS::Memory::EntityBlock entityBlock(descriptor, archetype);
	ASSERT_EQ(entityBlock.GetMaxEntityCount(), 25);
	free(data);
}
