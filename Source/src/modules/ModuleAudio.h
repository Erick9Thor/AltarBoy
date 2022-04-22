#pragma once
#include "Module.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>          // Memory Manager interface
#include <AK/SoundEngine/Common/AkModule.h>             // Default memory manager
#include <AK/SoundEngine/Common/IAkStreamMgr.h>         // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>            // Thread defines

#include <AK/SoundEngine/IO/AkFilePackageLowLevelIOBlocking.h> // Sample low-level I/O implementation

namespace Hachiko
{
    //class ComponentAudioSource;

    class ModuleAudio : public Module
    {
    public:
        ModuleAudio();
        ~ModuleAudio() override;

        bool Init() override;

    private:
        CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
    };
} // namespace Hachiko