#include "PreferenceManager.h"

#include "Globals.h"
#include "editor/CameraPreferences.h"
#include "editor/EditorPreferences.h"
#include "editor/GlobalPreferences.h"
#include "editor/RenderPreferences.h"
#include "editor/ResourcesPreferences.h"

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

    if (file_manager.CreateFileIfNew(SETTINGS_FOLDER))
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
    std::ofstream fout(CONFIG_PATH);
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
