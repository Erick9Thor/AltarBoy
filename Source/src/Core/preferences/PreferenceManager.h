#pragma once

#include "Preferences.h"
//#include "core/filesystem/FileManager.h"

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
        PreferenceManager(const char* cfg);
        ~PreferenceManager();
        void LoadConfigurationFile() const;
        void SaveConfigurationFile() const;
        [[nodiscard]] Preferences* GetPreference(Preferences::Type type) const;

    private:
        std::vector<Preferences*> preferences{};
        std::vector<YAML::Node> nodes_vec{};
        std::string config_file;

        GlobalPreferences* globals = nullptr;
        EditorPreferences* editor = nullptr;
        RenderPreferences* render = nullptr;
        CameraPreferences* camera = nullptr;
        ResourcesPreferences* resources = nullptr;
    };
}
