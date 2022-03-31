#include "core/hepch.h"
#include "ModuleScriptingSystem.h"
#include "utils/WindowsFileUtil.h"
#include "core/GameObject.h"
#include "scripting/Script.h"
#include "modules/ModuleFileSystem.h"
#include "Application.h"

constexpr const wchar_t* SCRIPTING_DLL_NAME = L"Gameplay";

Hachiko::ModuleScriptingSystem::ModuleScriptingSystem() 
    : _times_reloaded(0) 
    , _loaded_dll(NULL) 
    , _current_dll_timestamp("") 
    , _current_dll_name(L"")
    , _dll_change_check_timer(0.0f)
    , _dll_change_check_frequency_in_secs(0.0f)
    , _script_factory(nullptr)
{
}

bool Hachiko::ModuleScriptingSystem::Init()
{
    HE_LOG("ModuleScriptingSystem::Init");

    bool loading_successful = true;

    _dll_change_check_frequency_in_secs = 2.0f;
    _dll_change_check_timer = _dll_change_check_frequency_in_secs;

    _dummy_game_object = new GameObject("Dummy");

    HotReload(0.0f);

    // TODO: if loading is unsuccessful, log something like "Couldn't find the 
    // scripting dll, continuing without scripts.

    return true;
}

bool Hachiko::ModuleScriptingSystem::CleanUp()
{
    if (_dummy_script != nullptr)
    {
        delete _dummy_script;
    }

    FreeDll(_loaded_dll, _times_reloaded);

    delete _dummy_game_object;


    return true;
}

UpdateStatus Hachiko::ModuleScriptingSystem::PreUpdate(const float delta)
{
    HotReload(delta);

    HE_LOG("ModuleScriptingSystem::PreUpdate");

    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleScriptingSystem::Update(const float delta)
{
    HE_LOG("ModuleScriptingSystem::Update");

    _dummy_game_object->Update();

    if (_dummy_script != nullptr)
    {
        _dummy_script->Update();
    }

    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleScriptingSystem::PostUpdate(const float delta)
{
    HE_LOG("ModuleScriptingSystem::PostUpdate");

    return UpdateStatus::UPDATE_CONTINUE;
}

void Hachiko::ModuleScriptingSystem::HotReload(const float delta) 
{
    if (!ShouldCheckForChanges(delta))
    {
        return;
    }

    if (!IsDllVersionChanged())
    {
        return;
    }

    // TODO: Serialize old dll.
    
    HMODULE new_dll;
    LoadDll(&new_dll);

    Scripting::ScriptFactory new_factory = 
        reinterpret_cast<Scripting::ScriptFactory>(
            GetProcAddress(new_dll, "InstantiateScript"));

    _dummy_script = new_factory(_dummy_game_object, "PlayerController");

    // TODO: Deserialize to new dll.

    FreeDll(_loaded_dll, _times_reloaded - 1);
    _loaded_dll = new_dll;
    _script_factory = new_factory;
}

bool Hachiko::ModuleScriptingSystem::ShouldCheckForChanges(const float delta)
{
    if (_dll_change_check_timer >= _dll_change_check_frequency_in_secs)
    {
        _dll_change_check_timer = 0.0f;
        
        return true;
    }

    _dll_change_check_timer += delta;

    return false;
}

void Hachiko::ModuleScriptingSystem::LoadDll(HMODULE* dll)
{
    // Increment this to hold an instance count of the original dll so we 
    // delete the previous one when we load the new one:
    _times_reloaded++;

    // Build the name of the new file that's gonna hold the copy of new dll:
    std::wstringstream wss;
    wss << L"_" << _times_reloaded << L".dll";
    std::wstring current_dll_name = SCRIPTING_DLL_NAME + wss.str();

    // Open the dll we will copy from, and dll we will use:
    std::ifstream source;
    std::ofstream destination(current_dll_name, std::ios::binary);

    // I couldn't solve this problem in a beautiful manner, for now I'm doing 
    // this to do other stuff. TODO: Find a robust solution to this.
    // Wait while the file is just created:
    int source_buffer_size = 0;

    std::wstring main_dll_name = SCRIPTING_DLL_NAME;
    main_dll_name = main_dll_name + L".dll";

    while (source_buffer_size <= 0)
    {
        // Wait for 100ms to be less busy-waiting:
        Sleep(100);

        source.close();
        source.open(main_dll_name, std::ios::binary);

        source_buffer_size = source.rdbuf()->
            pubseekoff(0, source.end, source.in);
    }

    // Go back to initial position:
    source.rdbuf()->pubseekpos(0, source.in);

    // Copy the original dll to the one we will use:
    destination << source.rdbuf();

    // Close streams:
    source.close();
    destination.close();

    // Load the copied library:
    *dll = LoadLibraryW(current_dll_name.c_str());
    _current_dll_name = current_dll_name;
}

void Hachiko::ModuleScriptingSystem::FreeDll(HMODULE dll, 
    unsigned int load_index) 
{
    // Form it's name using the same formula LoadDll uses:
    std::wstringstream wss;
    wss << L"_" << load_index << L".dll";
    std::wstring previous_dll_name = SCRIPTING_DLL_NAME + wss.str();

    // Free the dll, and delete it:
    FreeLibrary(dll);
    _wremove(previous_dll_name.c_str());
}

bool Hachiko::ModuleScriptingSystem::IsDllVersionChanged()
{
    std::string new_timestamp = "";

    std::wstring current_dll_path = App->file_sys->GetWorkingDirectoryW();
    current_dll_path += L"\\";
    current_dll_path += SCRIPTING_DLL_NAME;
    current_dll_path += L".dll";

    bool new_timestamp_loaded = GetFileLastWriteTimestamp(current_dll_path, 
        new_timestamp);

    if (!new_timestamp_loaded)
    {
        return false;
    }

    bool timestamp_changed = new_timestamp != _current_dll_timestamp;

    _current_dll_timestamp = new_timestamp;

    return timestamp_changed;
}
