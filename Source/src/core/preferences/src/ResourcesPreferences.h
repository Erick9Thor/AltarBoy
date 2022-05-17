#pragma once
#include "core/preferences/Preferences.h"
#include "resources/Resource.h"

namespace Hachiko
{
    class ResourcesPreferences final : public Preferences
    {
    public:
        ResourcesPreferences();
        ~ResourcesPreferences() override = default;
        void LoadConfigurationData(const YAML::Node& node) override;
        void SaveConfigurationData(YAML::Node& node) override;
        [[nodiscard]] static const char* GetAssetsPath(Resource::AssetType type);
        [[nodiscard]] static const char* GetLibraryPath(Resource::Type type);
        static const std::map<Resource::AssetType, std::string>& GetAssetsPathsMap()
        {
            return assets_paths;
        };
        static const std::map<Resource::Type, std::string>& GetLibraryPathsMap()
        {
            return lib_paths;
        };
        [[nodiscard]] const char* GetSceneName() const
        {
            return scene_name.c_str();
        }

        void SetSceneName(const char* name) 
        {
            scene_name = name;
        }


    private:
        static const std::map<Resource::AssetType, std::string> assets_paths;
        static const std::map<Resource::Type, std::string> lib_paths;
        std::string scene_name = "UnnamedScene.scene";
        UID scene_id;
    };
}