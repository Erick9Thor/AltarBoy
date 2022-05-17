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
    }
}

void ResourcesPreferences::SaveConfigurationData(YAML::Node& node)
{
    node[group_name][SCENE_NAME] = scene_name + SCENE_EXTENSION;
}

const char* ResourcesPreferences::GetAssetsPath(Resource::Type type)
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
