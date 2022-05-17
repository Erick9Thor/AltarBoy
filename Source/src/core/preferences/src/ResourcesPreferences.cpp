#include "core/hepch.h"
#include "ResourcesPreferences.h"

#include <cassert>

using namespace Hachiko;

ResourcesPreferences::ResourcesPreferences()
    : Preferences(Type::RESOURCE_PREFS)
{
    group_name = RESOURCES_NODE;
}

void ResourcesPreferences::LoadConfigurationData(const YAML::Node& node)
{
    for (auto it = node.begin(); it != node.end(); ++it)
    {
        
        std::string node_name = it->first.as<std::string>();
        std::string node_value = it->second.as<std::string>();
        // Scene
        if (node_name._Equal(SCENE_NAME))
        {
            scene_name = std::move(node_value);
            continue;
        }
    }
}

void ResourcesPreferences::SaveConfigurationData(YAML::Node& node)
{
    node[group_name][SCENE_NAME] = scene_name + SCENE_EXTENSION;
    node[group_name][SCENE_ID] = scene_id;
    // We skip unkwnown type
    for (auto& asset_type : GetAssetsPathsMap())
    {
        node[group_name][Resource::AssetTypeString(static_cast<Resource::AssetType>(asset_type.first)) + ASSETS_COMMON_NAME] = asset_type.second;
    }

    for (auto& resource_type : GetLibraryPathsMap())
    {
        node[group_name][Resource::ResourceTypeString(static_cast<Resource::Type>(resource_type.first)) + LIBRARY_COMMON_NAME] = resource_type.second;
    }
}

const char* ResourcesPreferences::GetAssetsPath(Resource::AssetType type) const
{
    auto it = assets_paths.find(type);
    if (it == assets_paths.end())
    {
        assert(false);
    }
    return it->second.c_str();
}

const char* ResourcesPreferences::GetLibraryPath(Resource::Type type) const
{
    auto it = lib_paths.find(type);
    if (it == lib_paths.end())
    {
        assert(false);
    }
    return it->second.c_str();
}
