#pragma once
#include "Module.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>          // Memory Manager interface
#include <AK/SoundEngine/Common/AkModule.h>             // Default memory manager
#include <AK/SoundEngine/Common/IAkStreamMgr.h>         // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>            // Thread defines
#include <AK/SoundEngine/Common/AkSoundEngine.h>        // Sound engine
#include <AK/MusicEngine/Common/AkMusicEngine.h>        // Music Engine
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>      // Spatial Audio

// Include for communication between Wwise and the game -- Not needed in the release version
#ifndef AK_OPTIMIZED
    #include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

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
        UpdateStatus Update(float delta) override;
        bool CleanUp() override;

        void Play(const wchar_t* name_event) const;

    private:
        // We're using the default Low-Level I/O implementation that's part
        // of the SDK's sample code, with the file package extension
        CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

        // TODO: Monica will change this after VS2
        const AkGameObjectID GAME_OBJECT_ID_BGMUSIC = 1001;
        const AkGameObjectID GAME_OBJECT_ID_PLAYER = 1002;
    };
} // namespace Hachiko