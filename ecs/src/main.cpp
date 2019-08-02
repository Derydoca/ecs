#include "pch.h"
#include "systems.h"
#include <iostream>
#include <math.h>

int main()
{
	movementSystem movementSys;
	turningSystem turningSys;
	int numEntities = 50;
	
	position* positions = new position[numEntities];
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

	rotation* rotations = new rotation[numEntities];
	{
		float curr = 0;
		for (int i = 0; i < numEntities; i++)
		{
			rotations[i].value = curr;

			curr += 0.02f;
		}
	}

	speed* speeds = new speed[numEntities];
	{
		float curr = 0;
		for (int i = 0; i < numEntities; i++)
		{
			speeds[i].value = static_cast<float>(sin(curr));

			curr += 0.123f;
		}
	}

	while (true)
	{
		movementSys.run(numEntities, positions, rotations, speeds);
		turningSys.run(numEntities, rotations);
		std::cout << positions[5].x << ", " << positions[5].y << std::endl;
	}
	
	return 123;
}