#include "Timer.h"

void Timer::StartMicroseconds()
{
    start_time = SDL_GetPerformanceCounter();
}

double Timer::ReadMicroseconds()
{
    static const double frequency = (double)SDL_GetPerformanceFrequency();
    last_time = SDL_GetPerformanceCounter();
    return (double)((last_time - start_time) * 1000000) / frequency;
}

void Timer::StartMiliseconds()
{
    start_time = SDL_GetTicks();
}

float Timer::ReadMiliseconds()
{
    last_time = SDL_GetTicks();
    return (last_time - start_time) / 1000.0f;
}
