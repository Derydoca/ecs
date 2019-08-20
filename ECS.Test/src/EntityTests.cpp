#include "pch.h"
#include "Entity.h"

TEST(Entity, DefaultConstructor_Has_InvalidId) {
	ECS::Entity entity;
	ASSERT_EQ(entity.GetId(), ECS::Entity::INVALID_ENTITY_ID);
}

TEST(Entity, EntityReturnsGivenId) {
	ECS::Entity entity(25);
	ASSERT_EQ(entity.GetId(), 25);
}

TEST(Entity, EntitiesWithSameIds_Equals_Eachother)
{
	ASSERT_EQ(ECS::Entity(25), ECS::Entity(25));
}

TEST(Entity, EntitiesWithDifferentIds_NE_Eachother)
{
	ASSERT_NE(ECS::Entity(25), ECS::Entity(26));
}