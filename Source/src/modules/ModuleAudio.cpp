#include "core/hepch.h"
#include "ModuleAudio.h"

Hachiko::ModuleAudio::ModuleAudio() = default;

Hachiko::ModuleAudio::~ModuleAudio() = default;

bool Hachiko::ModuleAudio::Init()
{
    HE_LOG("Initializing Wwise");
    AkMemSettings memSettings;

    AK::MemoryMgr::GetDefaultSettings(memSettings);

    if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
    {
        assert(!"Could not create the memory manager.");
        return false;
    }

    return true;
}