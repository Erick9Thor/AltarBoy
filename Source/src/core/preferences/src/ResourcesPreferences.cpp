#include "core/hepch.h"
#include "ResourcesPreferences.h"

#include <cassert>

using namespace Hachiko;

ResourcesPreferences::ResourcesPreferences()
    : Preferences(Type::RESOURCES)
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

        if (node_name._Equal(SCENE_ID))
        {
            scene_id = std::move(node_value);
            continue;
        }

        // Assets paths
        for (auto& asset_it = assets_paths.begin(); asset_it != assets_paths.end(); ++asset_it)
        {
            if (node_name._Equal(asset_it->second))
            {
                assets_paths[asset_it->first] = node_value;
                continue;
            }
        }

        // Library paths
        for (auto& library_it = lib_paths.begin(); library_it != lib_paths.end(); ++library_it)
        {
            if (node_name._Equal(library_it->second))
            {
                lib_paths[library_it->first] = node_value;
                continue;
            }
        }

    }
}

void ResourcesPreferences::SaveConfigurationData(YAML::Node& node)
{
    node[group_name][SCENE_NAME] = scene_name;
    node[group_name][SCENE_ID] = scene_id;
    // We skip unkwnown type
    for (int type_val = static_cast<int>(Resource::Type::MATERIAL); type_val != static_cast<int>(Resource::Type::COUNT); ++type_val)
    {
        Resource::Type type = static_cast<Resource::Type>(type_val);
        node[group_name][Resource::TypeString(static_cast<Resource::Type>(type)) + ASSETS_COMMON_NAME] = assets_paths[type];
    }

    for (int type_val = static_cast<int>(Resource::Type::MATERIAL); type_val != static_cast<int>(Resource::Type::COUNT); ++type_val)
    {
        Resource::Type type = static_cast<Resource::Type>(type_val);
        node[group_name][Resource::TypeString(static_cast<Resource::Type>(type)) + LIBRARY_COMMON_NAME] = lib_paths[type];
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
