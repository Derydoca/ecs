#include "pch.h"
#include "Entity.h"
#include "components.h"
#include "systems.h"

TEST(Temporary, RotatingPoints) {
	MovementSystem movementSys;
	TurningSystem turningSys;

	int numEntities = 3;

	Position* positions = new Position[numEntities];
	positions[0] = { 0.0f, 0.0f };
	positions[1] = { 0.0f, 0.0f };
	positions[2] = { 0.0f, 0.0f };

	Rotation* rotations = new Rotation[numEntities];
	rotations[0] = { 0.0f };
	rotations[2] = { 0.123456f };
	rotations[1] = { 3.1415f };

	Speed* speeds = new Speed[numEntities];
	speeds[0] = { 1.0f };

	MovementSystemData movementData;
	movementData.count = numEntities;
	movementData.positions = positions;
	movementData.rotations = rotations;
	movementData.speeds = speeds;

	TurningSystemData turningData;
	turningData.count = numEntities;
	turningData.rotations = rotations;

	movementSys.Run(movementData);
	turningSys.Run(turningData);
}
