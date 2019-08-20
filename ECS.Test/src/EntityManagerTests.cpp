#include "pch.h"
#include "EntityManager.h"
#include "EntityManagerDebugInfo.h"

TEST(EntityManager, FirstGeneratedId_Equals_One) {
	ECS::EntityManager manager(0, 0);
	ECS::Entity entity0;
	manager.CreateEntity(entity0);
	ASSERT_EQ(entity0.GetId(), 1);
}

TEST(EntityManager, SecondGeneratedId_Equals_Two) {
	ECS::EntityManager manager(0, 0);
	ECS::Entity entity0;
	ECS::Entity entity1;
	manager.CreateEntity(entity0);
	manager.CreateEntity(entity1);
	ASSERT_EQ(entity1.GetId(), 2);
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