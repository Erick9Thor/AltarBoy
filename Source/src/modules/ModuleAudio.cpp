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
    HE_LOG("Wwise: Memory manager complete");

    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings(stmSettings);

    // Customize the Stream Manager settings here.
    if (!AK::StreamMgr::Create(stmSettings))
    {
        assert(!"Could not create the Streaming Manager");
        return false;
    }
    HE_LOG("Wwise: Streaming Manager complete");

    // Create a streaming device with blocking low-level I/O handshaking.
    // Note that you can override the default low-level I/O module with your own.

    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    // Customize the streaming device settings here.
    // CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
    // in the Stream Manager, and registers itself as the File Location Resolver.

    if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
    {
        assert(!"Could not create the streaming device and Low-Level I/O system");
        return false;
    }
    HE_LOG("Wwise: Streaming device and Low-Level I/O system complete");

    // Create the Sound Engine
    // Using default initialization parameters
    AkInitSettings initSettings;
    AkPlatformInitSettings platformInitSettings;
    AK::SoundEngine::GetDefaultInitSettings(initSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

    if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
    {
        assert(!"Could not initialize the Sound Engine.");
        return false;
    }
    HE_LOG("Wwise: Sound Engine complete");

    // Initialize the music engine
    // Using default initialization parameters
    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings(musicInit);

    if (AK::MusicEngine::Init(&musicInit) != AK_Success)
    {
        assert(!"Could not initialize the Music Engine.");
        return false;
    }
    HE_LOG("Wwise: Music Engine complete");

    // Initialize Spatial Audio
    // Using default initialization parameters
    AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings.

    if (AK::SpatialAudio::Init(settings) != AK_Success)
    {
        assert(!"Could not initialize the Spatial Audio.");
        return false;
    }
    HE_LOG("Wwise: Spatial Audio complete");

#ifndef AK_OPTIMIZED
    // Initialize communications (not in release build!)
    AkCommSettings commSettings;
    AK::Comm::GetDefaultInitSettings(commSettings);

    if (AK::Comm::Init(commSettings) != AK_Success)
    {
        assert(!"Could not initialize communication.");
        return false;
    }
    HE_LOG("Wwise: Communications initialized");
#endif // AK_OPTIMIZED

    return true;
}

UpdateStatus Hachiko::ModuleAudio::Update(const float delta) 
{
    AK::SoundEngine::RenderAudio();
    return UpdateStatus::UPDATE_CONTINUE;
}

bool Hachiko::ModuleAudio::CleanUp()
{
#ifndef AK_OPTIMIZED
    // Terminate Communication Services
    AK::Comm::Term();
#endif // AK_OPTIMIZED

    // Terminate Spatial Audio
    //AK::SpatialAudio::Term();

    // Terminate the music engine
    AK::MusicEngine::Term();

    // Terminate the sound engine
    AK::SoundEngine::Term();

    // Terminate the streaming device and streaming manager
    // CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device
    // that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
    g_lowLevelIO.Term();

    if (AK::IAkStreamMgr::Get())
    {
        AK::IAkStreamMgr::Get()->Destroy();
    }

    // Terminate the Memory Manager
    AK::MemoryMgr::Term();

    return true;
}