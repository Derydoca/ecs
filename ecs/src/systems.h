#pragma once
#include "components.h"

struct MovementSystemData
{
	unsigned int count;
	Position* positions;
	Rotation* rotations;
	Speed* speeds;
};

class MovementSystem
{
public:
	void Run(MovementSystemData& data);
};

struct TurningSystemData
{
	unsigned int count;
	Rotation* rotations;
};

class TurningSystem
{
public:
	void Run(TurningSystemData& data);
};