#include "pch.h"
#include "EntityLocation.h"

TEST(EntityLocation, BlockIndex_Equals_ConstructedValue)
{
	ECS::EntityLocation location = ECS::EntityLocation(5, 25);
	ASSERT_EQ(location.GetBlockIndex(), 5);
}

TEST(EntityLocation, EntityIndex_Equals_ConstructedValue)
{
	ECS::EntityLocation location = ECS::EntityLocation(5, 25);
	ASSERT_EQ(location.GetEntityIndex(), 25);
}

TEST(EntityLocation, TwoLocations_NE_WhenOnlyBlockIndexMatches)
{
	ASSERT_NE(ECS::EntityLocation(5, 1), ECS::EntityLocation(5, 25));
}

TEST(EntityLocation, TwoLocations_NE_WhenOnlyEntityIndexMatches)
{
	ASSERT_NE(ECS::EntityLocation(1, 25), ECS::EntityLocation(5, 25));
}

TEST(EntityLocation, TwoIdenticalLocations_Equal_Eachother)
{
	ASSERT_EQ(ECS::EntityLocation(5, 25), ECS::EntityLocation(5, 25));
}

TEST(EntityLocation, SetValues_Equals_ConstructedValues)
{
	ECS::EntityLocation location = ECS::EntityLocation(0, 0);
	ECS::EntityLocation expectedLocation = ECS::EntityLocation(5, 25);
	location.Set(5, 25);
	ASSERT_EQ(location, expectedLocation);
}

TEST(EntityLocation, IsInvalidLocation_When_ResetToInvalidIsCalled)
{
	ECS::EntityLocation location = ECS::EntityLocation(5, 25);
	location.ResetToInvalid();
	ASSERT_EQ(location, ECS::EntityLocation::INVALID_LOCATION);
}