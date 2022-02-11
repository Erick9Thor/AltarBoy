#pragma once

#include "Window.h"
#include "Utils/Timer.h"

class WindowTimers : public Window
{
public:
	WindowTimers();
	~WindowTimers() override;

	void Update() override;

private:
	Timer timer;
};
