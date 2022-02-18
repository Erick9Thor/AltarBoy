#include "Core/hepch.h"
#include "PreferenceManager.h"

#include "Globals.h"
#include "src/CameraPreferences.h"
#include "src/EditorPreferences.h"
#include "src/GlobalPreferences.h"
#include "src/RenderPreferences.h"
#include "src/ResourcesPreferences.h"

#include "Modules/ModuleFileSystem.h"

using namespace Hachiko;

PreferenceManager::PreferenceManager()
      : globals(new GlobalPreferences())
      , editor(new EditorPreferences())
      , render(new RenderPreferences())
      , camera(new CameraPreferences())
      , resources(new ResourcesPreferences())
{
    preferences.reserve(static_cast<size_t>(Preferences::Type::COUNT));

    preferences.emplace_back(globals);
    preferences.emplace_back(render);
    preferences.emplace_back(editor);
    preferences.emplace_back(camera);
    preferences.emplace_back(resources);

    if (App->file_sys->Exists(SETTINGS_FOLDER))
    {
        nodes_vec = YAML::LoadAllFromFile(SETTINGS_FOLDER);
        LoadConfigurationFile();
    }
}

PreferenceManager::~PreferenceManager()
{
    for (const auto it : preferences)
    {
        delete it;
    }
    preferences.clear();
}

void PreferenceManager::LoadConfigurationFile() const
{
    for (auto node : nodes_vec)
    {
        for (auto it : preferences)
        {
            if (!node[it->GetGroupName()].IsDefined())
            {
                continue;
            }
            it->SetConfigurationData(node[it->GetGroupName()]);
        }
    }
}

void PreferenceManager::SaveConfigurationFile() const
{
    YAML::Node output;
    for (const auto it : preferences)
    {
        it->GetConfigurationData(output);
    }
    std::ofstream fout(SETTINGS_FOLDER);
    fout << output;
}

Preferences* PreferenceManager::GetPreferenceDataByType(const Preferences::Type type) const
{
    for (const auto it : preferences)
    {
        if (it->GetType() == type)
        {
            return it;
        }
    }
    return nullptr;
}

Preferences* PreferenceManager::GetEditorPreferences() const
{
    return editor;
}
