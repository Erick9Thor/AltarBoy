#include "Timer.h"

#include "SDL.h"

void Timer::Start()
{
	running = true;
	start_time = SDL_GetTicks();
}

double Timer::Read()
{
	if (running)
	{
		unsigned int now = SDL_GetTicks();
		current_time = (double) (now - start_time); // ms
	}
	return current_time;
}

double Timer::Stop()
{
	return Read();
	running = false;
}

void Timer::StartGame()
{
	if (game_started) return;
	game_started = true;
	game_running = true;
}

void Timer::StopGame()
{
	if (!game_started) return;
	game_started = false;
	game_running = false;
}

void Timer::PauseGame()
{
	if (!game_started) return;
	if (!game_running) return;

	game_running = false;
}

void Timer::ResumeGame()
{
	if (!game_started) return;
	if (game_running) return;

	game_running = true;
}

void Timer::StepGame()
{
	if (!game_started) StartGame();
	if (game_running) PauseGame();

	game_step_once = true;
}

void PerformanceTimer::Start()
{
	running = true;
	start_time = SDL_GetPerformanceCounter();
}

double PerformanceTimer::Read()
{
	static const double frequency = (double) SDL_GetPerformanceFrequency();
	if (running)
	{
		unsigned long long now = SDL_GetPerformanceCounter();
		current_time = (double) ((now - start_time) * 1000.0 / frequency);
	}
	return current_time;
}

double PerformanceTimer::Stop()
{
	return Read();
	running = false;
}
