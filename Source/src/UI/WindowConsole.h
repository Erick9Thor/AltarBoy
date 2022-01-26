#pragma once
#include "Window.h"

#include <imgui.h>


class WindowConsole : public Window
{
public:
	WindowConsole();
	~WindowConsole() override;
	void Update() override;
	void Clear();

	void CleanUp() override;

private:
	ImGuiTextFilter filter = nullptr;
	bool autoscroll = true;
};
