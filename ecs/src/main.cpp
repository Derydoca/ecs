#include "pch.h"
#include "systems.h"
#include <iostream>
#include <math.h>

int main()
{
	MovementSystem movementSys;
	TurningSystem turningSys;

	int numEntities = 50;
	
	Position* positions = new Position[numEntities];
	{
		float currx = 0;
		float curry = 0;
		for (int i = 0; i < numEntities; i++)
		{
			positions[i].x = currx;
			positions[i].y = curry;

			currx += 0.01f;
			curry += 0.01f;
		}
	}

	Rotation* rotations = new Rotation[numEntities];
	{
		float curr = 0;
		for (int i = 0; i < numEntities; i++)
		{
			rotations[i].value = curr;

			curr += 0.02f;
		}
	}

	Speed* speeds = new Speed[numEntities];
	{
		float curr = 0;
		for (int i = 0; i < numEntities; i++)
		{
			speeds[i].value = static_cast<float>(sin(curr));

			curr += 0.123f;
		}
	}

	MovementSystemData movementData;
	movementData.count = numEntities;
	movementData.positions = positions;
	movementData.rotations = rotations;
	movementData.speeds = speeds;

	TurningSystemData turningData;
	turningData.count = numEntities;
	turningData.rotations = rotations;

	while (true)
	{
		movementSys.Run(movementData);
		turningSys.Run(turningData);
		std::cout << positions[5].x << ", " << positions[5].y << std::endl;
	}
	
	return 0;
}