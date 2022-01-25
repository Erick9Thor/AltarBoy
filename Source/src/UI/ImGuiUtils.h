#pragma once

#include "imgui.h"
#include "imgui_internal.h"

using namespace ImGui;

namespace ImGuiUtils
{
	bool IconButton(const char* icon, const char* tooltip)
	{
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		PushStyleColor(ImGuiCol_Button, GetStyle().Colors[ImGuiCol_WindowBg]);
		bool res = SmallButton(icon);
		if (IsItemHovered())
		{
			SetTooltip("%s", tooltip);
		}
		PopStyleColor();
		PopStyleVar();
		return res;
	}

	void VSplitter(const char* str_id, ImVec2* size)
	{
		ImVec2 screen_pos = GetCursorScreenPos();
		InvisibleButton(str_id, ImVec2(3, -1));
		ImVec2 end_pos = screen_pos + GetItemRectSize();
		ImGuiWindow* win = GetCurrentWindow();
		ImVec4* colors = GetStyle().Colors;
		ImU32 color = GetColorU32(ImGui::IsItemActive() || IsItemHovered() ? colors[ImGuiCol_ButtonActive] : colors[ImGuiCol_Button]);
		win->DrawList->AddRectFilled(screen_pos, end_pos, color);
		if (IsItemActive())
		{
			size->x = ImMax(1.0f, GetIO().MouseDelta.x + size->x);
		}
	}

	void Rect(float w, float h, ImU32 color)
	{
		ImGuiWindow* win = GetCurrentWindow();
		ImVec2 screen_pos = GetCursorScreenPos();
		ImVec2 end_pos = screen_pos + ImVec2(w, h);
		ImRect total_bb(screen_pos, end_pos);
		ItemSize(total_bb);
		if (!ItemAdd(total_bb, 0)) return;
		win->DrawList->AddRectFilled(screen_pos, end_pos, color);
	}
};
