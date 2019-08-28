#include "pch.h"
#include "EntityBlock.h"

class EntityBlockTest : public ::testing::Test {
protected:
	EntityBlockTest(
		size_t blockSize = 256,
		ECS::EntityArchetype archetype = ECS::EntityArchetype(ECS::tid<int>(), ECS::tid<float>())
	) :
		archetype(archetype),
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
	size_t entityAndArchetypeSize = archetype.GetEntitySize() + sizeof(ECS::Entity);
	size_t maxEntityCount = (blockSize / entityAndArchetypeSize);
	ASSERT_EQ(entityBlock.GetMaxEntityCount(), maxEntityCount);
}

TEST_F(EntityBlockTest, BlockSize_GE_HeaderAndEntitySize) {
	size_t headerOffset = blockSize / archetype.GetEntitySize();
	size_t maxEntityCount = (blockSize - headerOffset) / archetype.GetEntitySize();
	ASSERT_GE(blockSize, headerOffset + (entityBlock.GetMaxEntityCount() * archetype.GetEntitySize()));
}

TEST_F(EntityBlockTest, NewBlockWithAssign_Equals_ConstructorVariant)
{
	auto newBlock = ECS::Memory::EntityBlock();
	newBlock.Assign(descriptor, archetype);
	ASSERT_EQ(entityBlock.GetDescriptor(), newBlock.GetDescriptor());
	ASSERT_EQ(entityBlock.GetArchetype(), newBlock.GetArchetype());
	ASSERT_EQ(entityBlock.GetMaxEntityCount(), newBlock.GetMaxEntityCount());
}

TEST_F(EntityBlockTest, EntityID_Equals_ConstructedID_When_InsertedAndRetrieved)
{
	entityBlock.InsertEntity(5, ECS::Entity(25));
	ASSERT_EQ(entityBlock.GetEntity(5).GetId(), 25);
}

TEST_F(EntityBlockTest, EntityID_Equals_InvalidEntity_When_EntityIsDeleted)
{
	entityBlock.InsertEntity(5, ECS::Entity(25));
	entityBlock.DeleteEntity(5);
	int deletedEntityId = entityBlock.GetEntity(5).GetId();
	ASSERT_EQ(deletedEntityId, ECS::Entity::INVALID_ENTITY_ID);
}

TEST_F(EntityBlockTest, AllEntities_Equal_Invalid_When_BlockIsConstructed)
{
	ECS::Entity* entities = reinterpret_cast<ECS::Entity*>(entityBlock.GetDescriptor().m_data);
	for (size_t i = 0; i < entityBlock.GetMaxEntityCount(); i++)
	{
		ASSERT_EQ(entities[i], ECS::Entity::INVALID_ENTITY_ID);
	}
}

TEST_F(EntityBlockTest, AllDataAfterEntities_Equal_Zero_When_BlockIsConstructed)
{
	char* data = entityBlock.GetDescriptor().m_data;
	for (size_t i = entityBlock.GetMaxEntityCount() * sizeof(ECS::Entity); i < entityBlock.GetDescriptor().m_blockSize; i++)
	{
		ASSERT_EQ(data[i], char(0));
	}
}

TEST(EntityBlock, MaxEntityCount_Equals_CapacityOfEntireBlock_When_ArchetypeHasNoTypes)
{
	ECS::Memory::MemoryBlockDescriptor blockDescriptor = ECS::Memory::MemoryBlockDescriptor(25, 256, nullptr);
	size_t expectedMaxEntityCount = blockDescriptor.m_blockSize / sizeof(ECS::Entity);
	ECS::Memory::EntityBlock block = ECS::Memory::EntityBlock(blockDescriptor, ECS::EntityArchetype());
	ASSERT_EQ(block.GetMaxEntityCount(), expectedMaxEntityCount);
}