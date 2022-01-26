#pragma once

#include "Window.h"

class WindowAbout : public Window
{
public:
	WindowAbout();
	~WindowAbout() override;
	void Update() override;
};
