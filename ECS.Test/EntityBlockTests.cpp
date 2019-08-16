#include "pch.h"
#include "EntityBlock.h"

class EntityBlockTest : public ::testing::Test {
protected:
	EntityBlockTest(size_t blockSize = 256) :
		archetype(ECS::tid<int>(), ECS::tid<float>()),
		blockSize(blockSize),
		data(0),
		descriptor(25, blockSize, data),
		entityBlock(descriptor, archetype)
	{}

	ECS::EntityArchetype archetype;
	size_t blockSize = 256;
	char* data;
	ECS::Memory::MemoryBlockDescriptor descriptor;
	ECS::Memory::EntityBlock entityBlock;

	void SetUp() override
	{
		archetype = (ECS::tid<int>(), ECS::tid<float>());
		blockSize = 256;
		data = reinterpret_cast<char*>(malloc(blockSize));
		descriptor = ECS::Memory::MemoryBlockDescriptor(25, blockSize, data);
		entityBlock = ECS::Memory::EntityBlock(descriptor, archetype);
	}

	void TearDown() override
	{
		free(data);
	}

};

TEST_F(EntityBlockTest, ReturnedDescriptor_Equals_ConstructorDescriptor) {
	ASSERT_EQ(entityBlock.GetDescriptor(), descriptor);
}

TEST_F(EntityBlockTest, ReturnedArchetype_Equals_ConstructorArchetype) {
	ASSERT_EQ(entityBlock.GetArchetype(), archetype);
}

TEST_F(EntityBlockTest, MaxEntityCount_Equals_ExpectedAmount) {
	size_t headerOffset = blockSize / archetype.GetEntitySize();
	size_t maxEntityCount = (blockSize - headerOffset) / archetype.GetEntitySize();
	ASSERT_EQ(entityBlock.GetMaxEntityCount(), maxEntityCount);
}

TEST_F(EntityBlockTest, BlockSize_GE_HeaderAndEntitySize) {
	size_t headerOffset = blockSize / archetype.GetEntitySize();
	size_t maxEntityCount = (blockSize - headerOffset) / archetype.GetEntitySize();
	ASSERT_GE(blockSize, headerOffset + (entityBlock.GetMaxEntityCount() * archetype.GetEntitySize()));
}

TEST_F(EntityBlockTest, EntityData_Equals_InsertedData) {
	char* entityData = reinterpret_cast<char*>(malloc(archetype.GetEntitySize()));
	for (size_t i = 0; i < archetype.GetEntitySize(); i++)
	{
		entityData[i] = static_cast<char>(i);
	}
	entityBlock.InsertEntityData(2, entityData);
	char* blockEntityData = entityBlock.GetEntityMemoryAddress(2);
	ASSERT_TRUE(memcmp(entityData, blockEntityData, archetype.GetEntitySize()) == 0);
	free(entityData);
}

TEST_F(EntityBlockTest, EntityData_Equals_Zero_When_Deleted) {
	char* entityData = reinterpret_cast<char*>(malloc(archetype.GetEntitySize()));
	for (size_t i = 0; i < archetype.GetEntitySize(); i++)
	{
		entityData[i] = static_cast<char>(i);
	}
	entityBlock.InsertEntityData(2, entityData);
	entityBlock.DeleteEntityData(2);
	char* blockEntityData = entityBlock.GetEntityMemoryAddress(2);
	memset(entityData, 0, archetype.GetEntitySize());
	ASSERT_TRUE(memcmp(entityData, blockEntityData, archetype.GetEntitySize()) == 0);
	free(entityData);
}

TEST_F(EntityBlockTest, EntityData_NotEquals_SourceData_When_Deleted) {
	char* entityData = reinterpret_cast<char*>(malloc(archetype.GetEntitySize()));
	for (size_t i = 0; i < archetype.GetEntitySize(); i++)
	{
		entityData[i] = static_cast<char>(i);
	}
	entityBlock.InsertEntityData(2, entityData);
	entityBlock.DeleteEntityData(2);
	char* blockEntityData = entityBlock.GetEntityMemoryAddress(2);
	ASSERT_TRUE(memcmp(entityData, blockEntityData, archetype.GetEntitySize()) != 0);
	free(entityData);
}
