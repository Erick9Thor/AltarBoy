#pragma once

#include "imgui.h"
#include "imgui_internal.h"
namespace ImGuiUtils
{
	bool IconButton(const char* icon, const char* tooltip);

	void VSplitter(const char* str_id, ImVec2* size);

	void Rect(float w, float h, ImU32 color);

	bool CompactColorPicker(const char* name, float* color);
};
