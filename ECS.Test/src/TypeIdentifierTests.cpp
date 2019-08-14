#include "pch.h"
#include "TypeIdentifier.h"

TEST(TypeIdentifier, IntTypeGeneratesID) {
	ECS::TypeIdentifier id = ECS::GetTypeIdentifier<int>();
	ASSERT_NE(id, 0);
}

TEST(TypeIdentifier, FloatTypeGeneratesID) {
	ECS::TypeIdentifier id = ECS::GetTypeIdentifier<float>();
	ASSERT_NE(id, 0);
}

TEST(TypeIdentifier, IntAndFloatTypesDoNotEqual) {
	ECS::TypeIdentifier intTypeId = ECS::GetTypeIdentifier<int>();
	ECS::TypeIdentifier floatTypeId = ECS::GetTypeIdentifier<float>();
	ASSERT_NE(intTypeId, floatTypeId);
}
