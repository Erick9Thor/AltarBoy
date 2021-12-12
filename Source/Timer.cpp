#include "Timer.h"

#include "Leaks.h"

Timer::Timer()
{
	Start();
}

void Timer::Start()
{
	running = true;
	start_time = SDL_GetTicks();
}

void Timer::Stop()
{
	running = false;
	last_time = SDL_GetTicks();
}

int Timer::Read()
{
	if (running == true)
	{
		return SDL_GetTicks() - start_time;
	}
	else
	{
		return last_time - start_time;
	}
}
