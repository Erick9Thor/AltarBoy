#include "Globals.h"
#include "core/hepch.h"
#include "utils/Logger.h"

void LogFunction(const char file[], int line, const char* format, ...) 
{
    va_list args;
    va_start(args, format);
    Logging->log(file, line, format, args);
    va_end(args);
}