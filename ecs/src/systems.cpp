#include "pch.h"
#include "systems.h"
#include <math.h>

void movementSystem::run(movementSystemData& data)
{
	for (unsigned int i = 0; i < data.count; i++)
	{
		float dx = cos(data.rotations[i].value) * data.speeds[i].value;
		float dy = sin(data.rotations[i].value) * data.speeds[i].value;
		data.positions[i].x += dx;
		data.positions[i].y += dy;
	}
}

void turningSystem::run(turningSystemData& data)
{
	for (unsigned int i = 0; i < data.count; i++)
	{
		data.rotations[i].value += 0.005f;
	}
}
