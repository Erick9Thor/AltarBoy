#pragma once
#include "Window.h"

class WindowConfiguration : public Window
{
public:
	WindowConfiguration();
	~WindowConfiguration() override;
	void Update() override;
};
