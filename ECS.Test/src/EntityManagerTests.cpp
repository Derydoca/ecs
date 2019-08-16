#include "pch.h"
#include "EntityManager.h"

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
	ECS::EntityManager manager(0, 0);
	ECS::Entity entity0;
	manager.CreateEntity(entity0);
	ASSERT_DEATH(manager.CreateEntity(entity0), "");
}
