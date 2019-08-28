#include "pch.h"
#include "EntityManager.h"
#include "EntityManagerDebugInfo.h"

TEST(EntityManager, FirstGeneratedId_Equals_Zero) {
	ECS::EntityManager manager(0, 0);
	ECS::Entity entity0;
	manager.CreateEntity(entity0);
	ASSERT_EQ(entity0.GetId(), 0);
}

TEST(EntityManager, SecondGeneratedId_Equals_One) {
	ECS::EntityManager manager(0, 0);
	ECS::Entity entity0;
	ECS::Entity entity1;
	manager.CreateEntity(entity0);
	manager.CreateEntity(entity1);
	ASSERT_EQ(entity1.GetId(), 1);
}

TEST(EntityManager, ExpectAssert_When_AttemptingToGenerateEntityTwice) {
	ECS::EntityManager manager(128, 1);
	ECS::Entity entity0;
	manager.CreateEntity(entity0);
	ASSERT_DEATH(manager.CreateEntity(entity0), "");
}

TEST(EntityManager, EntityCount_Equals_One_When_AnEntityIsCreated)
{
	ECS::EntityManager manager(128, 1);
	auto debugInfo = ECS::EntityManagerDebugInfo(&manager);
	manager.CreateEntity(ECS::Entity());
	size_t entityCount = debugInfo.GetEntityCount();
	ASSERT_EQ(entityCount, 1);
}

TEST(EntityManager, EntityCount_Equals_Two_When_TwoEntitiesAreCreated)
{
	ECS::EntityManager manager(128, 1);
	auto debugInfo = ECS::EntityManagerDebugInfo(&manager);
	manager.CreateEntity(ECS::Entity());
	manager.CreateEntity(ECS::Entity());
	size_t entityCount = debugInfo.GetEntityCount();
	ASSERT_EQ(entityCount, 2);
}

TEST(EntityManager, EntityCount_Equals_Zero_When_AnEntityIsCreatedAndDeleted)
{
	ECS::EntityManager manager(128, 1);
	auto debugInfo = ECS::EntityManagerDebugInfo(&manager);
	ECS::Entity entity;
	manager.CreateEntity(entity);
	manager.DeleteEntity(entity);
	size_t entityCount = debugInfo.GetEntityCount();
	ASSERT_EQ(entityCount, 0);
}

TEST(EntityManager, EntityCount_Equals_Zero_When_TwoEntitiesAreCreatedAndDeleted)
{
	ECS::EntityManager manager(128, 1);
	auto debugInfo = ECS::EntityManagerDebugInfo(&manager);
	ECS::Entity entity0;
	ECS::Entity entity1;
	manager.CreateEntity(entity0);
	manager.CreateEntity(entity1);
	manager.DeleteEntity(entity0);
	manager.DeleteEntity(entity1);
	size_t entityCount = debugInfo.GetEntityCount();
	ASSERT_EQ(entityCount, 0);
}

TEST(EntityManager, EntityBlockArchetype_Equals_EmptyArchetype_When_NoArchetypeIsSupplied)
{
	ECS::EntityManager manager(128, 1);
	auto debugInfo = ECS::EntityManagerDebugInfo(&manager);
	ECS::Entity entity;
	manager.CreateEntity(entity);
	ECS::EntityArchetype fetchedArchetype = debugInfo.GetEntityBlockArchetype(entity);
	ASSERT_EQ(fetchedArchetype, ECS::EntityArchetype::Empty);
}

TEST(EntityManager, EntityBlockArchetype_Equals_CreatedArchetype)
{
	ECS::EntityManager manager(128, 1);
	auto debugInfo = ECS::EntityManagerDebugInfo(&manager);
	ECS::EntityArchetype archetype = ECS::EntityArchetype(ECS::tid<int>(), ECS::tid<float>());
	ECS::Entity entity;
	manager.CreateEntity(entity, archetype);
	ECS::EntityArchetype fetchedArchetype = debugInfo.GetEntityBlockArchetype(entity);
	ASSERT_EQ(fetchedArchetype, archetype);
}

TEST(EntityManager, EntityBlockArchetype_Equals_ArchetypeWithFloatType_When_FloatTypeIsAddedToEntity)
{
	ECS::EntityManager manager(128, 2);
	auto debugInfo = ECS::EntityManagerDebugInfo(&manager);
	ECS::EntityArchetype archetype = ECS::EntityArchetype(ECS::tid<int>());
	ECS::Entity entity;
	manager.CreateEntity(entity, archetype);
	manager.AddComponentData(entity, ECS::tid<float>());
	ECS::EntityArchetype fetchedArchetype = debugInfo.GetEntityBlockArchetype(entity);
	ECS::EntityArchetype expectedArchetype = ECS::EntityArchetype(ECS::tid<int>(), ECS::tid<float>());
	ASSERT_EQ(fetchedArchetype, expectedArchetype);
}

TEST(EntityManager, UsedBlockCount_Equals_One_When_AnEntityIsCreatedAndHadTypeAppendedToIt)
{
	ECS::EntityManager manager(128, 2);
	auto debugInfo = ECS::EntityManagerDebugInfo(&manager);
	ECS::EntityArchetype archetype = ECS::EntityArchetype(ECS::tid<int>());
	ECS::Entity entity;
	manager.CreateEntity(entity, archetype);
	manager.AddComponentData(entity, ECS::tid<float>());
	ASSERT_EQ(debugInfo.GetNumberOfBlocksInUse(), 2);
	manager.ReleaseEmptyBlocks();
	ASSERT_EQ(debugInfo.GetNumberOfBlocksInUse(), 1);
}

TEST(EntityManager, RetrievedEntityData_Equals_SetEntityData)
{
	struct TestPositionComponent
	{
		float x;
		float y;
	};

	ECS::EntityManager manager(128, 1);
	auto debugInfo = ECS::EntityManagerDebugInfo(&manager);
	ECS::EntityArchetype archetype = ECS::EntityArchetype(ECS::tid<TestPositionComponent>());
	ECS::Entity entity;
	manager.CreateEntity(entity, archetype);
	TestPositionComponent dataToSet = { 1.234f, 4.321f };
	manager.SetEntityData(entity, dataToSet);
	TestPositionComponent* retrievedData = manager.GetEntityData<TestPositionComponent>(entity);
	ASSERT_EQ(retrievedData->x, dataToSet.x);
	ASSERT_EQ(retrievedData->y, dataToSet.y);
}

struct Position2D
{
	float m_x, m_y;
	Position2D() {}
	Position2D(float x, float y) : m_x(x), m_y(y) {}
};

void MoveSystem(ECS::EntityManager* manager, ECS::Entity entity, char* data00)
{
	Position2D* positionComponent = reinterpret_cast<Position2D*>(data00);
	positionComponent->m_x += 0.1f;
	positionComponent->m_y += 0.2f;
}

TEST(EntityManager, SystemTest)
{

	ECS::EntityManager manager(128, 2);
	auto debugInfo = ECS::EntityManagerDebugInfo(&manager);
	ECS::EntityArchetype archetype = ECS::EntityArchetype(ECS::tid<Position2D>());
	for (size_t i = 0; i < 20; i++)
	{
		ECS::Entity entity;
		manager.CreateEntity(entity, archetype);
		manager.SetEntityData<Position2D>(entity, Position2D(static_cast<float>(i), static_cast<float>(i)));
	}
	ASSERT_EQ(debugInfo.GetNumberOfBlocksInUse(), 2);
	manager.TransformData<Position2D>(MoveSystem);
	for (size_t i = 0; i < 20; i++)
	{
		ECS::Entity entity(static_cast<int>(i));
		Position2D* position = manager.GetEntityData<Position2D>(entity);
		ASSERT_EQ(position->m_x, static_cast<float>(i) + 0.1f);
		ASSERT_EQ(position->m_y, static_cast<float>(i) + 0.2f);
	}
}
