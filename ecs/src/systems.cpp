#include "pch.h"
#include "systems.h"
#include <math.h>

void MovementSystem::Run(MovementSystemData& data)
{
	for (unsigned int i = 0; i < data.count; i++)
	{
		float dx = static_cast<float>(cos(data.rotations[i].value)) * data.speeds[i].value;
		float dy = static_cast<float>(sin(data.rotations[i].value)) * data.speeds[i].value;
		data.positions[i].x += dx;
		data.positions[i].y += dy;
	}
}

void TurningSystem::Run(TurningSystemData& data)
{
	for (unsigned int i = 0; i < data.count; i++)
	{
		data.rotations[i].value += 0.005f;
	}
}
