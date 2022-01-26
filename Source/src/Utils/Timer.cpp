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
}


void PerformanceTimer::Start()
{
	running = true;
	start_time = SDL_GetPerformanceCounter();
}

void PerformanceTimer::Resume()
{
	if (!running)
	{
		running = true;
		start_time = SDL_GetPerformanceCounter() - (stop_time - start_time);
	}
}

double PerformanceTimer::Read()
{
	static const double frequency = (double) SDL_GetPerformanceFrequency();

	unsigned long long now;
	if (running)
		now = SDL_GetPerformanceCounter();
	else
		now = stop_time;

	current_time = (double) ((now - start_time) * 1000.0 / frequency);
	return current_time;
}

double PerformanceTimer::Stop()
{
	running = false;
	stop_time = SDL_GetPerformanceCounter();
	return Read();	
}

double GameTimer::delta_time = 0.;
double GameTimer::total_time = 0.;
double GameTimer::prev_tick_time = 0.;
bool GameTimer::running = false;
bool GameTimer::paused = false;
PerformanceTimer GameTimer::engine_timer;

void GameTimer::Start()
{
	total_time = 0.;
	running = true;
	paused = false;
	engine_timer.Start();
	prev_tick_time = engine_timer.Read();
}

double GameTimer::Update()
{
	double tick_time = engine_timer.Read();
	delta_time = (tick_time - prev_tick_time) / 1000.0;
	prev_tick_time = tick_time;
	if (running)
		total_time = engine_timer.Read();
	return delta_time;
}

void GameTimer::Play()
{
	running = true;
}

void GameTimer::Pause()
{
	paused = true;
	engine_timer.Stop();
}

void GameTimer::Resume()
{
	paused = false;
	engine_timer.Resume();
}

void GameTimer::Stop()
{
	running = false;
	engine_timer.Stop();
}
