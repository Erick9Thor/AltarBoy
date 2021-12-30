#pragma once
#include "Window.h"
#include <imgui.h>

class WindowScene : public Window
{
public:
	WindowScene();
	void Update() override;
	void ToolbarButton(ImFont* font, const char* font_icon);
};
