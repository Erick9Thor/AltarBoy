#include "scriptingUtil/gameplaypch.h"
#include "CameraShake.h"
#include <math.h>

Hachiko::Scripting::CameraShake::CameraShake()
{
	magnitude = 0.0f;
}

void Hachiko::Scripting::CameraShake::Shake(float3* position)
{
	position.x += rand(-1, 1);
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}