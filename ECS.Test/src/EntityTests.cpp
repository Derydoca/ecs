#include "pch.h"
#include "Entity.h"

TEST(Entity, EntityReturnsGivenId) {
	ECS::Entity entity(25);
	ASSERT_EQ(entity.GetId(), 25);
}
