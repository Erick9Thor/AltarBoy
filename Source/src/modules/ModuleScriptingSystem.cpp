#include "core/hepch.h"
#include "ModuleScriptingSystem.h"

bool Hachiko::ModuleScriptingSystem::Init()
{
    HE_LOG("ModuleScriptingSystem::Init");

    bool loading_successful = true;

    // TODO: Load dll here for the first time here, and if loading is unsuccess
    // -ful, log something like "Couldn't find the scripting dll, continuing wi
    // -thout scripts.

    return true;
}

UpdateStatus Hachiko::ModuleScriptingSystem::PreUpdate(const float delta)
{
    HE_LOG("ModuleScriptingSystem::PreUpdate");

    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleScriptingSystem::Update(const float delta)
{
    HE_LOG("ModuleScriptingSystem::Update");

    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus Hachiko::ModuleScriptingSystem::PostUpdate(const float delta)
{
    HE_LOG("ModuleScriptingSystem::PostUpdate");

    return UpdateStatus::UPDATE_CONTINUE;
}
