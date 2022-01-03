#pragma once
#include "Window.h"

#include <imgui.h>


class WindowConsole : public Window
{
public:
	WindowConsole();
	void Update() override;
	void Clear();

private:
	ImGuiTextFilter filter;
	bool autoscroll = true;
};
