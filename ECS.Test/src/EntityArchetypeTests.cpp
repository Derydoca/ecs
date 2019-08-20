#include "pch.h"
#include "EntityArchetype.h"

TEST(EntityArchetype, EntityArchetype_Int_HasSize) {
	ECS::EntityArchetype archetype(ECS::tid<int>());
	ASSERT_GT(archetype.GetEntitySize(), 0);
}

TEST(EntityArchetype, EntityArchetype_IntInt_HasLargerSizeThan_Int) {
	ECS::EntityArchetype archetypeInt(ECS::tid<int>());
	ECS::EntityArchetype archetypeIntInt(ECS::tid<int>(), ECS::tid<int>());
	ASSERT_GT(archetypeIntInt.GetEntitySize(), archetypeInt.GetEntitySize());
}

TEST(EntityArchetype, EntityArchetype_IntFloat_DoesNotEqual_FloatInt) {
	ECS::EntityArchetype archetypeIntFloat(ECS::tid<int>(), ECS::tid<float>());
	ECS::EntityArchetype archetypeFloatInt(ECS::tid<float>(), ECS::tid<int>());
	ASSERT_NE(archetypeIntFloat, archetypeFloatInt);
}

TEST(EntityArchetype, EntityArchetype_IntFloat_Equals_IntFloat) {
	ECS::EntityArchetype archetypeIntFloat1(ECS::tid<int>(), ECS::tid<float>());
	ECS::EntityArchetype archetypeIntFloat2(ECS::tid<int>(), ECS::tid<float>());
	ASSERT_EQ(archetypeIntFloat1, archetypeIntFloat2);
}

TEST(EntityArchetype, EntityArchetype_IntFloat_Equals_FloatInt_WithIgnoreFlagOn) {
	ECS::EntityArchetype archetypeIntFloat(ECS::tid<int>(), ECS::tid<float>());
	ECS::EntityArchetype archetypeFloatInt(ECS::tid<float>(), ECS::tid<int>());
	ASSERT_TRUE(archetypeIntFloat.Equals(archetypeFloatInt, ECS::EntityArchetypeComparisonFlags::IgnoreOrder));
}

TEST(EntityArchetype, EntityArchetype_IntFloat_DoesNotEqual_FloatInt_WithIgnoreFlagOff) {
	ECS::EntityArchetype archetypeIntFloat(ECS::tid<int>(), ECS::tid<float>());
	ECS::EntityArchetype archetypeFloatInt(ECS::tid<float>(), ECS::tid<int>());
	ASSERT_FALSE(archetypeIntFloat.Equals(archetypeFloatInt, ECS::EntityArchetypeComparisonFlags::None));
}

TEST(EntityArchetype, AppendedArchetype_Equals_MatchingConstructedArchetype)
{
	ECS::EntityArchetype originalArchetype(ECS::tid<int>(), ECS::tid<float>());
	ECS::EntityArchetype expectedArchetype(ECS::tid<int>(), ECS::tid<float>(), ECS::tid<char>());
	ASSERT_EQ(ECS::EntityArchetype::CreateArchetypeWithNewType(originalArchetype, ECS::tid<char>()), expectedArchetype);
}