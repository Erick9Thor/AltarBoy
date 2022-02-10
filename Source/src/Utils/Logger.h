#pragma once

#include <imgui.h>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

class Logger
{
public:
	Logger();
	~Logger();
	void log(const char file[], int line, const char* format, ...);
	ImVector<int> line_offsets;
	ImGuiTextBuffer buff;
};


extern Logger* Logging;


