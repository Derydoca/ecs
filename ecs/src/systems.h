#pragma once
#include "components.h"

struct movementSystemData
{
	unsigned int count;
	position* positions;
	rotation* rotations;
	speed* speeds;
};

class movementSystem
{
public:
	void run(movementSystemData& data);
};

struct turningSystemData
{
	unsigned int count;
	rotation* rotations;
};

class turningSystem
{
public:
	void run(turningSystemData& data);
};