#include "pch.h"
#include "TypeIdentifier.h"

TEST(TypeIdentifier, IntTypeGeneratesID) {
	ECS::TID id = ECS::tid<int>();
	ASSERT_NE(id.GetId(), 0);
}

TEST(TypeIdentifier, IntTypeGeneratesSize) {
	ECS::TID id = ECS::tid<int>();
	ASSERT_GT(id.GetSize(), 0);
}

TEST(TypeIdentifier, FloatTypeGeneratesID) {
	ECS::TID id = ECS::tid<float>();
	ASSERT_NE(id.GetId(), 0);
}

TEST(TypeIdentifier, FloatTypeGeneratesSize) {
	ECS::TID id = ECS::tid<float>();
	ASSERT_GT(id.GetSize(), 0);
}

TEST(TypeIdentifier, IntAndFloatTypesDoNotEqual) {
	ECS::TID intTypeId = ECS::tid<int>();
	ECS::TID floatTypeId = ECS::tid<float>();
	ASSERT_NE(intTypeId.GetId(), floatTypeId.GetId());
}

TEST(TypeIdentifier, TwoIntTIDsAreEqual) {
	ECS::TID int1TypeId = ECS::tid<int>();
	ECS::TID int2TypeId = ECS::tid<int>();
	ASSERT_EQ(int1TypeId.GetId(), int2TypeId.GetId());
}
