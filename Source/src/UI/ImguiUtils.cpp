#include "ImGuiUtils.h"
#include <IconsFontAwesome5.h>

#include "../GameObject.h"
#include "../Components/Component.h"

using namespace ImGui;
using namespace std;

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

void ImGuiUtils::Rect(float w, float h, ImU32 color)
{
	ImGuiWindow* win = GetCurrentWindow();
	ImVec2 screen_pos = GetCursorScreenPos();
	ImVec2 end_pos = screen_pos + ImVec2(w, h);
	ImRect total_bb(screen_pos, end_pos);
	ItemSize(total_bb);
	if (!ItemAdd(total_bb, 0)) return;
	win->DrawList->AddRectFilled(screen_pos, end_pos, color);
}

bool ImGuiUtils::CompactColorPicker(const char* name, float* color)
{
	constexpr ImGuiColorEditFlags flags = ImGuiColorEditFlags_DisplayRGB |ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
	PushItemWidth(150.0f);
	bool ret = ColorPicker4(name, color, flags);
	PopItemWidth();
	return ret;
}

bool ImGuiUtils::CollapsingHeader(GameObject* game_object, Component* component, const char* header_name)
{
	bool open = ImGui::CollapsingHeader(header_name, ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);

	ImGui::SameLine();
	if (ImGui::GetWindowWidth() > 170) ImGui::Indent(ImGui::GetWindowWidth() - 43);
	if (ImGui::Button(std::string(ICON_FA_TIMES).c_str())) ImGui::OpenPopup(header_name);
	if (ImGui::GetWindowWidth() > 170) ImGui::Unindent(ImGui::GetWindowWidth() - 43);

	if (ImGui::BeginPopup(header_name))
	{
		game_object->RemoveComponent(component);
		ImGui::EndPopup();
	}

	return open;
}


