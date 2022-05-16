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
        [[nodiscard]] const char* GetAssetsPath(Resource::Type type);
        [[nodiscard]] const char* GetLibraryPath(Resource::Type type) const;
        [[nodiscard]] const char* GetSceneName() const
        {
            return scene_name.c_str();
        }

        void SetSceneName(const char* name) 
        {
            scene_name = name;
        }

    private:
        std::map<Resource::Type, std::string> assets_paths = {
            {Resource::Type::SCENE, "assets/scenes/"}, 
            {Resource::Type::MODEL, "assets/models/"},
            {Resource::Type::MESH, "assets/meshes/"},
            {Resource::Type::TEXTURE, "assets/textures/"},
            {Resource::Type::AUDIO, "assets/audio/"},
            {Resource::Type::VIDEO, "assets/video/"},
            {Resource::Type::SCRIPT, "assets/scripts/"},
            {Resource::Type::MATERIAL, "assets/materials/"},
            {Resource::Type::SHADER, "assets/shaders/"},
            {Resource::Type::ANIMATION, "assets/animations/"},
            {Resource::Type::SKYBOX, "assets/skybox/"},
            {Resource::Type::FONT, "assets/fonts/"},
            {Resource::Type::NAVMESH, "assets/navmesh/"},
        };

        std::map<Resource::Type, std::string> lib_paths = {
            {Resource::Type::SCENE, "library/scenes/"},
            {Resource::Type::MODEL, "library/models/"},
            {Resource::Type::MESH, "library/meshes/"},
            {Resource::Type::TEXTURE, "library/textures/"},
            {Resource::Type::AUDIO, "library/audio/"},
            {Resource::Type::VIDEO, "library/video/"},
            {Resource::Type::SCRIPT, "library/scripts/"},
            {Resource::Type::MATERIAL, "library/materials/"},
            {Resource::Type::SHADER, "library/shaders/"},
            {Resource::Type::ANIMATION, "library/animations/"},
            {Resource::Type::SKYBOX, "library/skybox/"},
            {Resource::Type::FONT, "library/fonts/"},
            {Resource::Type::NAVMESH, "library/navmesh/"},
        };
        std::string scene_name        = "UnnamedScene.scene";
        std::string scene_id;
    };
}