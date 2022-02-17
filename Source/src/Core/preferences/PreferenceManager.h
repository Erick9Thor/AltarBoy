#pragma once

#include "Preferences.h"
#include "core/filesystem/FileManager.h"

namespace Hachiko
{
    class GlobalPreferences;
    class EditorPreferences;
    class RenderPreferences;
    class CameraPreferences;
    class ResourcesPreferences;

    class PreferenceManager
    {
    public:
        PreferenceManager();
        ~PreferenceManager();
        void LoadConfigurationFile() const;
        void SaveConfigurationFile() const;
        [[nodiscard]] Preferences* GetPreferenceDataByType(Preferences::Type type) const;
        [[nodiscard]] Preferences* GetEditorPreferences() const;

    private:
        std::vector<Preferences*> preferences{};
        std::vector<YAML::Node> nodes_vec{};
        //FileManager file_manager{};

        GlobalPreferences* globals = nullptr;
        EditorPreferences* editor = nullptr;
        RenderPreferences* render = nullptr;
        CameraPreferences* camera = nullptr;
        ResourcesPreferences* resources = nullptr;
    };
}
