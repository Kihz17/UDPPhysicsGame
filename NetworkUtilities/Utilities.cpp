#include "Utilities.h"

#include <stdlib.h>
#include <stdio.h>

float Utils::GetRandomFloat(float low, float high)
{
	return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
}

int Utils::GetRandomInt(int low, int high)
{
	return low + static_cast<int>(rand()) / (static_cast<int>(RAND_MAX) / (high - low));
}