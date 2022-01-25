#include "ImGuiUtils.h"


using namespace ImGui;

bool ImGuiUtils::IconButton(const char* icon, const char* tooltip)
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

void ImGuiUtils::VSplitter(const char* str_id, ImVec2* size)
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

bool ImGuiUtils::CompactColorPicker(const char* name, float* color)
{
	constexpr ImGuiColorEditFlags flags = ImGuiColorEditFlags_DisplayRGB |ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
	PushItemWidth(150.0f);
	bool ret = ColorPicker4(name, color, flags);
	PopItemWidth();
	return ret;
}