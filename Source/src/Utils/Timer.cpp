#include "Timer.h"

#include "SDL.h"

void Timer::Start()
{
	running = true;
	start_time = SDL_GetTicks();
}

void Timer::Resume()
{
	if (!running)
	{
		running = true;
		start_time = SDL_GetTicks() - (stop_time - start_time);
	}
}

double Timer::Read()
{
	unsigned int now;
	if (running)
		now = SDL_GetTicks();
	else
		now = stop_time;
		
	current_time = (double) (now - start_time); // ms
	return current_time;
}

double Timer::Stop()
{
	if (running)
	{
		running = false;
		stop_time = SDL_GetTicks();
		return Read();
	}
	return (double) stop_time;	
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
	if (running)
	{
		running = false;
		stop_time = SDL_GetPerformanceCounter();
		return Read();	
	}
	return (double) stop_time;
}

double GameTimer::delta_time = 0.;
double GameTimer::total_time = 0.;
double GameTimer::prev_tick_time = 0.;
bool GameTimer::running = false;
bool GameTimer::paused = false;
PerformanceTimer GameTimer::timer;

void GameTimer::Start()
{
	total_time = 0.;
	running = true;
	paused = false;
	timer.Start();
	prev_tick_time = timer.Read();
}

double GameTimer::Update()
{
	double tick_time = timer.Read();
	delta_time = (tick_time - prev_tick_time) / 1000.0;
	prev_tick_time = tick_time;
	if (running)
		total_time = timer.Read();
	return delta_time;
}

void GameTimer::Play()
{
	running = true;
}

void GameTimer::Pause()
{
	paused = true;
	timer.Stop();
}

void GameTimer::Resume()
{
	paused = false;
	timer.Resume();
}

void GameTimer::Stop()
{
	running = false;
	timer.Stop();
}

PerformanceTimer  EngineTimer::timer;
double EngineTimer::delta_time = 0.;
double EngineTimer::total_time = 0.;
double EngineTimer::prev_tick_time = 0.;
bool EngineTimer::running = false;


void EngineTimer::Start()
{
	total_time = 0.;
	running = true;
	timer.Start();
	prev_tick_time = timer.Read();
}

double EngineTimer::Update()
{
	double tick_time = timer.Read();
	delta_time = (tick_time - prev_tick_time) / 1000.0;
	prev_tick_time = tick_time;
	if (running)
		total_time = timer.Read();
	return delta_time;
}
