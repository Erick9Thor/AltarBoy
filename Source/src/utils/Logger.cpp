#include "core/hepch.h"
#include "Logger.h"

Hachiko::Logger::Logger() = default;

Hachiko::Logger::~Logger() = default;

void Hachiko::Logger::log(const char file[], int line, const char* format, ...)
{
    static char tmp_string[4096];
    static char tmp_string2[4096];
    static char tmp_string3[4096];

    int old_size = buff.size();
    va_list args;
    va_start(args, format);
    vsprintf_s(tmp_string, 4096, format, args);
    va_end(args);
    sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);

    buff.append(tmp_string2);

    for (const int new_size = buff.size(); old_size < new_size; old_size++)
        if (buff[old_size] == '\n')
            line_offsets.push_back(old_size + 1);
    // TODO MONICA: Change this to work with unicode
    //OutputDebugString(tmp_string2);
}
