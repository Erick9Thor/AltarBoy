#pragma once

#include <imgui.h>
#include "HachikoApiDefine.h"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

namespace Hachiko
{
    class Logger
    {
    public:
        Logger();
        ~Logger();
        void log(const char file[], int line, const char* format, ...);
        ImVector<int> line_offsets;
        ImGuiTextBuffer buff;
    };
}

extern HACHIKO_API Hachiko::Logger* Logging;