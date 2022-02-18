#include "Core/hepch.h"
#include "ResourcesPreferences.h"

#include <cassert>

using namespace Hachiko;

ResourcesPreferences::ResourcesPreferences() : Preferences(Type::RESOURCES)
{
    group_name = RESOURCES_NODE;
}

void ResourcesPreferences::SetConfigurationData(const YAML::Node& node)
{
    for (auto it = node.begin(); it != node.end(); ++it)
    {
        // Assets path
        if (it->first.as<std::string>()._Equal(SCENE_ASSETS))
        {
            scene_assets = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(MODELS_ASSETS))
        {
            models_assets = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(MESHES_ASSETS))
        {
            meshes_assets = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(TEXTURES_ASSETS))
        {
            textures_assets = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(AUDIO_ASSETS))
        {
            audio_assets = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(VIDEO_ASSETS))
        {
            video_assets = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(SCRIPTS_ASSETS))
        {
            script_assets = std::move(it->second.as<std::string>());
            continue;
        }

        // Library path
        if (it->first.as<std::string>()._Equal(SCENE_LIBRARY))
        {
            meshes_library = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(MODELS_LIBRARY))
        {
            models_library = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(MESHES_LIBRARY))
        {
            meshes_library = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(TEXTURES_LIBRARY))
        {
            textures_library = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(AUDIO_LIBRARY))
        {
            audio_library = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(VIDEO_LIBRARY))
        {
            video_library = std::move(it->second.as<std::string>());
            continue;
        }

        if (it->first.as<std::string>()._Equal(SCRIPTS_LIBRARY))
        {
            script_library = std::move(it->second.as<std::string>());
        }
    }
}

void ResourcesPreferences::GetConfigurationData(YAML::Node& node)
{
    node[group_name][SCENE_ASSETS] = scene_assets;
    node[group_name][MODELS_ASSETS] = models_assets;
    node[group_name][MESHES_ASSETS] = meshes_assets;
    node[group_name][TEXTURES_ASSETS] = textures_assets;
    node[group_name][AUDIO_ASSETS] = audio_assets;
    node[group_name][VIDEO_ASSETS] = video_assets;
    node[group_name][SCRIPTS_ASSETS] = script_assets;

    node[group_name][SCENE_LIBRARY] = scene_library;
    node[group_name][MODELS_LIBRARY] = models_library;
    node[group_name][MESHES_LIBRARY] = meshes_library;
    node[group_name][TEXTURES_LIBRARY] = textures_library;
    node[group_name][AUDIO_LIBRARY] = audio_library;
    node[group_name][VIDEO_LIBRARY] = video_library;
    node[group_name][SCRIPTS_LIBRARY] = script_library;
}

const char* ResourcesPreferences::GetAssetsPath(Resource::Type type)
{
    switch (type)
    {
    case Resource::Type::SCENE:
        return scene_assets.c_str();
    case Resource::Type::MODEL:
        return models_assets.c_str();
    case Resource::Type::MESH:
        return meshes_assets.c_str();
    case Resource::Type::TEXTURE:
        return textures_assets.c_str();
    case Resource::Type::AUDIO:
        return audio_assets.c_str();
    case Resource::Type::ANIMATION:
        return video_assets.c_str();
    case Resource::Type::SCRIPT:
        return script_assets.c_str();
    case Resource::Type::UNKNOWN:
    default:
        assert(false);
    }
}

const char* ResourcesPreferences::GetLibraryPath(Resource::Type type) const
{
    switch (type)
    {
    case Resource::Type::SCENE:
        return scene_library.c_str();
    case Resource::Type::MODEL:
        return models_library.c_str();
    case Resource::Type::MESH:
        return meshes_library.c_str();
    case Resource::Type::TEXTURE:
        return textures_library.c_str();
    case Resource::Type::AUDIO:
        return audio_library.c_str();
    case Resource::Type::ANIMATION:
        return video_library.c_str();
    case Resource::Type::SCRIPT:
        return script_library.c_str();
    case Resource::Type::UNKNOWN:
    default:
        assert(false);
    }
}
