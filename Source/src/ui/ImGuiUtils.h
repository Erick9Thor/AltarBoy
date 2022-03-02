#pragma once

#include "imgui.h"

namespace Hachiko
{
    class GameObject;
    class Component;

    namespace ImGuiUtils
    {
        bool IconButton(const char* icon, const char* tooltip);

        void VSplitter(const char* str_id, ImVec2* size);

        void Rect(float w, float h, ImU32 color);

        bool CompactColorPicker(const char* name, float* color);

        bool CollapsingHeader(GameObject* game_object, Component* component, const char* header_name);

        bool ToolbarButton(ImFont* font, const char* font_icon, bool active, const char* tooltip_desc);

        static void DisplayTooltip(const char* desc);

        ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs);

    }; // namespace ImGuiUtils
}
