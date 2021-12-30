#pragma once

class Timer
{
public:
	void Start();
	double Read();
	double Stop();

	void StartGame();
	void StopGame();
	void PauseGame();
	void ResumeGame();
	void StepGame();

	bool HasGameStarted() const
	{
		return game_started;
	}

	bool IsGameRunning() const
	{
		return game_running;
	}

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
	double Read();
	double Stop();

private:
	bool running = false;
	unsigned long long start_time;
	double current_time;
};
