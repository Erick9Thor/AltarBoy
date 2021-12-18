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

