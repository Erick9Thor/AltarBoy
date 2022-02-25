#pragma once
#include "core/preferences/Preferences.h"

namespace Hachiko
{
    enum class ResourceType
    {
        SCENE = 0,
        MODEL,
        MESH,
        TEXTURE,
        AUDIO,
        VIDEO,
        SCRIPT,
        UNKNOWN,
    };

    class ResourcesPreferences final : public Preferences
    {
    public:
        ResourcesPreferences();
        ~ResourcesPreferences() override = default;
        void SetConfigurationData(const YAML::Node& node) override;
        void GetConfigurationData(YAML::Node& node) override;
        [[nodiscard]] const char* GetAssetsPath(Resource::Type type);
        [[nodiscard]] const char* GetLibraryPath(Resource::Type type) const;

    private:
        std::string scenes_assets     = "assets/scenes/";
        std::string models_assets     = "assets/models/";
        std::string meshes_assets     = "assets/meshes/";
        std::string textures_assets   = "assets/textures/";
        std::string audio_assets      = "assets/audio/";
        std::string video_assets      = "assets/video/";
        std::string script_assets     = "assets/scripts/";
        std::string materials_assets  = "assets/materials/";
        std::string shaders_assets    = "assets/shaders/";
        std::string animations_assets = "assets/animations/";
        std::string skybox_assets     = "assets/skybox/";

        std::string scenes_library     = "library/scenes/";
        std::string models_library     = "library/models/";
        std::string meshes_library     = "library/meshes/";
        std::string textures_library   = "library/textures/";
        std::string audio_library      = "library/audio/";
        std::string video_library      = "library/video/";
        std::string script_library     = "library/scripts/";
        std::string materials_library  = "library/materials/";
        std::string shaders_library    = "library/shaders/";
        std::string animations_library = "library/animations/";
        std::string skybox_library     = "library/skybox/";
    };
}