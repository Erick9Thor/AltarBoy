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
        [[nodiscard]] const char* GetAssetsPath(Resource::AssetType type) const;
        [[nodiscard]] const char* GetLibraryPath(Resource::Type type) const;
        const std::map<Resource::AssetType, std::string>& GetAssetsPathsMap()
        {
            return assets_paths;
        };
        const std::map<Resource::Type, std::string>& GetLibraryPathsMap()
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
        std::map<Resource::AssetType, std::string> assets_paths = {
            {Resource::AssetType::SCENE, "assets/scenes/"}, 
            {Resource::AssetType::MODEL, "assets/models/"},
            {Resource::AssetType::TEXTURE, "assets/textures/"},
            {Resource::AssetType::VIDEO, "assets/video/"},
            {Resource::AssetType::MATERIAL, "assets/materials/"},
            {Resource::AssetType::SHADER, "assets/shaders/"},
            {Resource::AssetType::SKYBOX, "assets/skybox/"},
            {Resource::AssetType::FONT, "assets/fonts/"},
            {Resource::AssetType::PREFAB, "assets/prefabs/"},
        };

        std::map<Resource::Type, std::string> lib_paths = {
            {Resource::Type::SCENE, "library/scenes/"},
            {Resource::Type::MESH, "library/meshes/"},
            {Resource::Type::TEXTURE, "library/textures/"},
            {Resource::Type::VIDEO, "library/video/"},
            {Resource::Type::MATERIAL, "library/materials/"},
            {Resource::Type::SHADER, "library/shaders/"},
            {Resource::Type::ANIMATION, "library/animations/"},
            {Resource::Type::SCRIPT, "library/scripts/"},
            {Resource::Type::SKYBOX, "library/skybox/"},
            {Resource::Type::FONT, "library/fonts/"},
            {Resource::Type::NAVMESH, "library/navmesh/"},
            {Resource::Type::PREFAB, "library/prefabs/"},
        };
        std::string scene_name = "UnnamedScene.scene";
        std::string scene_id;
    };
}