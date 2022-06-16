#pragma once

#include <imgui.h>
#include <string>

namespace Hachiko::Widgets
{
    inline bool Combo(const std::string& label, int* current_item, const char* const items[], int items_count)
    {
        ImGui::PushID(label.c_str());
        ImGui::PushID(items);
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 2));
        ImGui::BeginTable("", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoSavedSettings);
        ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_WidthStretch, 0.25f);
        ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_WidthStretch, 0.75f);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);

        ImGui::TextWrapped(label.c_str());
        ImGui::Spacing();

        ImGui::TableNextColumn();
        ImGui::PushItemWidth(-FLT_MIN);

        const bool value_changed = ImGui::Combo("##", current_item, items, items_count);

        ImGui::PopItemWidth();
        ImGui::EndTable();
        ImGui::PopStyleVar();
        ImGui::PopID();
        ImGui::PopID();
        return value_changed;
    }
}
