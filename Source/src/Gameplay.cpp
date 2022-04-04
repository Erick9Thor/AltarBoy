#include "core/hepch.h"
#include "utils/Logger.h"
#include "Gameplay.h"
#include "Time.h"

float Time::DeltaTime()
{
	// TODO: Return Gameplay Timer.
	// Added for easiness of exposing to scripts. We need to refactor that
	// timer code as it's unnecessarily complicated.
	return Hachiko::EngineTimer::delta_time;
}