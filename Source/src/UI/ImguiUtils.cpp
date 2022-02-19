#include "core/hepch.h"
#include "ImGuiUtils.h"
#include <IconsFontAwesome5.h>

#include "imgui_internal.h"

#include "core/GameObject.h"
#include "components/Component.h"

bool Hachiko::ImGuiUtils::IconButton(const char* icon, const char* tooltip)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
    const bool res = ImGui::SmallButton(icon);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("%s", tooltip);
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    return res;
}

void Hachiko::ImGuiUtils::VSplitter(const char* str_id, ImVec2* size)
{
    const ImVec2 screen_pos = ImGui::GetCursorScreenPos();
    ImGui::InvisibleButton(str_id, ImVec2(3, -1));
    const ImVec2 end_pos = screen_pos + ImGui::GetItemRectSize();
    const ImGuiWindow* win = ImGui::GetCurrentWindow();
    const ImVec4* colors = ImGui::GetStyle().Colors;
    const ImU32 color = ImGui::GetColorU32(ImGui::IsItemActive() || ImGui::IsItemHovered() ? colors[ImGuiCol_ButtonActive] : colors[ImGuiCol_Button]);
    win->DrawList->AddRectFilled(screen_pos, end_pos, color);
    if (ImGui::IsItemActive())
    {
        size->x = ImMax(1.0f, ImGui::GetIO().MouseDelta.x + size->x);
    }
}

void Hachiko::ImGuiUtils::Rect(float w, float h, ImU32 color)
{
    const ImGuiWindow* win = ImGui::GetCurrentWindow();
    const ImVec2 screen_pos = ImGui::GetCursorScreenPos();
    const ImVec2 end_pos = screen_pos + ImVec2(w, h);
    const ImRect total_bb(screen_pos, end_pos);
    ImGui::ItemSize(total_bb);
    if (!ImGui::ItemAdd(total_bb, 0))
    {
        return;
    }
    win->DrawList->AddRectFilled(screen_pos, end_pos, color);
}

bool Hachiko::ImGuiUtils::CompactColorPicker(const char* name, float* color)
{
    constexpr ImGuiColorEditFlags flags = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
    ImGui::PushItemWidth(150.0f);
    const bool ret = ImGui::ColorPicker4(name, color, flags);
    ImGui::PopItemWidth();
    return ret;
}

bool Hachiko::ImGuiUtils::CollapsingHeader(GameObject* game_object, Component* component, const char* header_name)
{
    const bool open = ImGui::CollapsingHeader(header_name, ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);

    ImGui::SameLine();
    if (ImGui::GetWindowWidth() > 170)
    {
        ImGui::Indent(ImGui::GetWindowWidth() - 43);
    }
    if (ImGui::Button(std::string(ICON_FA_TIMES).c_str()))
    {
        ImGui::OpenPopup(header_name);
    }
    if (ImGui::GetWindowWidth() > 170)
    {
        ImGui::Unindent(ImGui::GetWindowWidth() - 43);
    }

    if (ImGui::BeginPopup(header_name))
    {
        game_object->RemoveComponent(component);
        ImGui::EndPopup();
    }

    return open;
}

ImVec2 Hachiko::ImGuiUtils::operator+(const ImVec2& lhs, const ImVec2& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
