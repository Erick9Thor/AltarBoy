#include "core/hepch.h"
#include "ModuleScriptingSystem.h"
#include "utils/WindowsFileUtil.h"
#include "core/GameObject.h"
#include "scripting/Script.h"
#include "modules/ModuleFileSystem.h"
#include "modules/ModuleSceneManager.h"
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
    , _scripts_paused(false)
{
}

bool Hachiko::ModuleScriptingSystem::Init()
{
    _dll_change_check_frequency_in_secs = 2.0f;
    _dll_change_check_timer = _dll_change_check_frequency_in_secs;

    bool loading_successful = HotReload(0.0f);

    if (!loading_successful)
    {
        HE_LOG("There are no Gameplay.dll detected by Hachiko " 
            "Continuing without scripting functionality.");
    }

    return true;
}

bool Hachiko::ModuleScriptingSystem::CleanUp()
{
    if (_loaded_dll != NULL)
    {
        DeleteAllScriptsOnCurrentScene();

        FreeDll(_loaded_dll, _times_reloaded);
    }

    return true;
}

UpdateStatus Hachiko::ModuleScriptingSystem::PreUpdate(const float delta)
{
    HotReload(delta);

    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleScriptingSystem::Update(const float delta)
{
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleScriptingSystem::PostUpdate(const float delta)
{
    return UpdateStatus::UPDATE_CONTINUE;
}

Hachiko::Scripting::Script* Hachiko::ModuleScriptingSystem::InstantiateScript(
    const std::string& script_name, GameObject* owner_game_object) const
{
    if (_script_factory == nullptr)
    {
        return nullptr;
    }

    return _script_factory(owner_game_object, script_name);
}

bool Hachiko::ModuleScriptingSystem::HotReload(const float delta) 
{
    if (!ShouldCheckForChanges(delta))
    {
        return false;
    }

    if (!IsDllVersionChanged())
    {
        return false;
    }

    HMODULE new_dll = NULL;
    LoadDll(&new_dll);

    if (new_dll == NULL)
    {
        if (_loaded_dll != NULL)
        {
            HE_LOG("New loaded dll is NULL. Hot reloading is cancelled.");

            FreeDll(new_dll, _times_reloaded);
            _times_reloaded--;
        }
        else
        {
            HE_LOG("This code should never run.");
        }
        
        return false;
    }

    HE_LOG("Loaded Gameplay Scripting dll (%u).", _times_reloaded);

    Scripting::ScriptFactory new_factory = 
        reinterpret_cast<Scripting::ScriptFactory>(
            GetProcAddress(new_dll, "InstantiateScript"));

    // Get all the scripts inside the current scene:
    std::vector<Component*>& scripts = App->scene_manager->GetRoot()->
        GetComponentsInDescendants(Component::Type::SCRIPT);
    
    std::unordered_map<std::string, Scripting::SerializedField> serialization;    

    for (Component* component : scripts)
    {
        // Get current as script:
        Scripting::Script* script = static_cast<Scripting::Script*>(component);
        GameObject* game_object = script->GetGameObject();

        HE_LOG("Reloading %s of %s with id %llu", script->GetName().c_str(),
            game_object->name.c_str(), game_object->getUID());
        
        // Serialize script into the map:
        script->SerializeTo(serialization);
        HE_LOG("\tSerialized old version.");

        // Create new version of the script of same type:
        Scripting::Script* new_script = new_factory(game_object, 
            script->GetName().c_str());
        HE_LOG("\tCreated the new version.");

        // Remove the old script from it's owner:
        game_object->RemoveComponent(component);
        HE_LOG("\tRemoved the old version from components list.");

        if (new_script != nullptr)
        {
            // Deserialize old script to new script:
            new_script->DeserializeFrom(serialization);
            HE_LOG("\tDeserialized the old version to the new version.");

            // Add script to the game_object:
            game_object->AddComponent(new_script);
            HE_LOG("\tAdded the new version to components list.");
        }
        else
        {
            HE_LOG("\tScript '%s' could not be found in new dll.", 
                script->GetName().c_str());
        }

        // Delete the old script:
        delete script;
        HE_LOG("\tDeleted the old script.");
        
        component = new_script;

        // Clear the serialization map:
        serialization.clear();
    }

    // TODO: Call Awake of all the scripts if in game mode here.   

    FreeDll(_loaded_dll, _times_reloaded - 1);

    _loaded_dll = new_dll;
    
    _script_factory = new_factory;

    _scripts_paused = false;

    return true;
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

        source_buffer_size = source.rdbuf()->pubseekoff(0, source.end, 
            source.in);
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

void Hachiko::ModuleScriptingSystem::DeleteAllScriptsOnCurrentScene() const 
{
    // Get all the scripts inside the current scene:
    std::vector<Component*>& scripts = App->scene_manager->GetRoot()->
        GetComponentsInDescendants(Component::Type::SCRIPT);

    // TODO(Baran): Save all the scripts here also maybe?

    // We are deleting the scripts here because, scripts belong to dll and must
    // be deleted/freed before freeing the dll:
    for (Component* script : scripts)
    {
        script->GetGameObject()->RemoveComponent(script);
        
        delete script;
        
        script = nullptr;
    }

    scripts.clear();
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
