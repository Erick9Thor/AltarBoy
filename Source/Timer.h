#pragma once

#include "SDL.h"


class Timer
{
	public:
		
		Timer();

		void Start();
		void Stop();

		int Read();

	private:
		bool running;
		float start_time;
		float last_time = 0.0f;
};

