#pragma once

#include <imgui.h>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG(format, ...) log(__FILENAME__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

extern ImVector<int> line_offsets;
extern ImGuiTextBuffer buff;
