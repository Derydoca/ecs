#include "pch.h"
#include "EntityBlock.h"

TEST(EntityBlock, ReturnedDescriptor_Equals_ConstructorDescriptor) {
	ECS::EntityArchetype archetype(ECS::tid<int>(), ECS::tid<float>());
	size_t blockSize = 256;
	char* data = reinterpret_cast<char*>(malloc(blockSize));
	ECS::Memory::MemoryBlockDescriptor descriptor(25, blockSize, data);
	ECS::Memory::EntityBlock entityBlock(descriptor, archetype);
	ASSERT_EQ(entityBlock.GetDescriptor(), descriptor);
	free(data);
}

TEST(EntityBlock, ReturnedArchetype_Equals_ConstructorArchetype) {
	ECS::EntityArchetype archetype(ECS::tid<int>(), ECS::tid<float>());
	size_t blockSize = 256;
	char* data = reinterpret_cast<char*>(malloc(blockSize));
	ECS::Memory::MemoryBlockDescriptor descriptor(25, blockSize, data);
	ECS::Memory::EntityBlock entityBlock(descriptor, archetype);
	ASSERT_EQ(entityBlock.GetArchetype(), archetype);
	free(data);
}

TEST(EntityBlock, MaxEntityCount_Equals_ExpectedAmount) {
	ECS::EntityArchetype archetype(ECS::tid<int>(), ECS::tid<float>());
	size_t blockSize = archetype.GetEntitySize() * 25;
	char* data = reinterpret_cast<char*>(malloc(blockSize));
	ECS::Memory::MemoryBlockDescriptor descriptor(25, blockSize, data);
	ECS::Memory::EntityBlock entityBlock(descriptor, archetype);
	ASSERT_EQ(entityBlock.GetMaxEntityCount(), 25);
	free(data);
}

TEST(EntityBlock, EntityData_Equals_InsertedData) {
	ECS::EntityArchetype archetype(ECS::tid<int>(), ECS::tid<float>());
	char* entityData = reinterpret_cast<char*>(malloc(archetype.GetEntitySize()));
	for (size_t i = 0; i < archetype.GetEntitySize(); i++)
	{
		entityData[i] = static_cast<char>(i);
	}
	size_t blockSize = archetype.GetEntitySize() * 25;
	char* data = reinterpret_cast<char*>(malloc(blockSize));
	ECS::Memory::MemoryBlockDescriptor descriptor(25, blockSize, data);
	ECS::Memory::EntityBlock entityBlock(descriptor, archetype);
	entityBlock.InsertEntityData(2, entityData);
	char* blockEntityData = entityBlock.GetEntityMemoryAddress(2);
	ASSERT_TRUE(memcmp(entityData, blockEntityData, archetype.GetEntitySize()) == 0);
	free(entityData);
	free(data);
}

TEST(EntityBlock, EntityData_Equals_Zero_When_Deleted) {
	ECS::EntityArchetype archetype(ECS::tid<int>(), ECS::tid<float>());
	char* entityData = reinterpret_cast<char*>(malloc(archetype.GetEntitySize()));
	for (size_t i = 0; i < archetype.GetEntitySize(); i++)
	{
		entityData[i] = static_cast<char>(i);
	}
	size_t blockSize = archetype.GetEntitySize() * 25;
	char* data = reinterpret_cast<char*>(malloc(blockSize));
	ECS::Memory::MemoryBlockDescriptor descriptor(25, blockSize, data);
	ECS::Memory::EntityBlock entityBlock(descriptor, archetype);
	entityBlock.InsertEntityData(2, entityData);
	entityBlock.DeleteEntityData(2);
	char* blockEntityData = entityBlock.GetEntityMemoryAddress(2);
	memset(entityData, 0, archetype.GetEntitySize());
	ASSERT_TRUE(memcmp(entityData, blockEntityData, archetype.GetEntitySize()) == 0);
	free(entityData);
	free(data);
}
