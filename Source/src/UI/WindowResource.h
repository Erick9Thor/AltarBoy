#pragma once
#include "Window.h"

class WindowResource : public Window
{
public:
	WindowResource();
	~WindowResource() override; 
	void Update() override;
};
