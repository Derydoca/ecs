#include "pch.h"
#include "systems.h"
#include <math.h>

void movementSystem::run(unsigned int count, position* positions, rotation* rotations, speed* speeds)
{
	for (unsigned int i = 0; i < count; i++)
	{
		float dx = cos(rotations[i].value) * speeds[i].value;
		float dy = sin(rotations[i].value) * speeds[i].value;
		positions[i].x += dx;
		positions[i].y += dy;
	}
}

void turningSystem::run(unsigned int count, rotation* rotations)
{
	for (unsigned int i = 0; i < count; i++)
	{
		rotations[i].value += 0.005f;
	}
}
