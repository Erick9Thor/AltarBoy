#pragma once

#include "SDL.h"


class Timer
{
	public:
		void StartMicroseconds();
		double ReadMicroseconds();

		void StartMiliseconds();
		float ReadMiliseconds();
	private:
		float start_time;
		float last_time = 0.0f;
};

