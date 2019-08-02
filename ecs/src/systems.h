#pragma once
#include "components.h"

class movementSystem
{
public:
	void run(unsigned int count, position* positions, rotation* rotations, speed* speeds);
};

class turningSystem
{
public:
	void run(unsigned int count, rotation* rotations);
};