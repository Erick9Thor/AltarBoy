#pragma once
#include "Window.h"

#include <imgui.h>

namespace Hachiko
{
    class WindowConsole final : public Window
    {
    public:
        WindowConsole();
        ~WindowConsole() override;
        void Update() override;
        static void Clear();

        void CleanUp() override;

    private:
        ImGuiTextFilter filter = nullptr;
        bool autoscroll = true;
    };
}
