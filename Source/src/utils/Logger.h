#pragma once

#include <imgui.h>
#include "HachikoApiDefine.h"

namespace Hachiko
{
    class Logger
    {
    public:
        Logger();
        ~Logger();
        void log(const char file[], int line, const char* format, va_list args);
        ImVector<int> line_offsets;
        ImGuiTextBuffer buff;
    };
}

extern Hachiko::Logger* Logging;