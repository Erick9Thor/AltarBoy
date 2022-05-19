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
    for (int type_val = 1; type_val != static_cast<int>(Resource::AssetType::COUNT); ++type_val)
    {
        Resource::AssetType asset_type = static_cast<Resource::AssetType>(type_val);
        node[group_name][Resource::AssetTypeString(static_cast<Resource::AssetType>(asset_type)) + ASSETS_COMMON_NAME] = assets_paths[asset_type];
    }

    for (int type_val = 1; type_val != static_cast<int>(Resource::Type::COUNT); ++type_val)
    {
        Resource::Type resource_type = static_cast<Resource::Type>(type_val);
        node[group_name][Resource::ResourceTypeString(static_cast<Resource::Type>(resource_type)) + LIBRARY_COMMON_NAME] = lib_paths[resource_type];
    }
}

const char* ResourcesPreferences::GetAssetsPath(Resource::AssetType type)
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
