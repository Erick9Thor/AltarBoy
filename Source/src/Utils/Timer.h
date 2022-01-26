#pragma once

class Timer
{
public:
	void Start();
	double Read();
	double Stop();
	
private:
	bool running = false;
	unsigned int start_time;
	double current_time;

	bool game_started = false;
	bool game_running = false;
	bool game_step_once = false;
};

class PerformanceTimer
{
public:
	void Start();
	void Resume();
	double Read();
	double Stop();

private:
	bool running = false;
	unsigned long long start_time;
	unsigned long long stop_time;
	double current_time;
};


class GameTimer
{
public:

	static void Start();
	static double Update();

	static void Play();
	static void Pause();
	static void Resume();
	static void Stop();

	static PerformanceTimer engine_timer;

	static double delta_time;
	static double total_time;
	static double prev_tick_time;

	static bool running;
	static bool paused;
};
