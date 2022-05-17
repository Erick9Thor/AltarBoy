#include "core/hepch.h"
#include "ResourcesPreferences.h"

#include <cassert>

using namespace Hachiko;

const std::map<Resource::AssetType, std::string> ResourcesPreferences::assets_paths = {
    {Resource::AssetType::SCENE, ASSETS_FOLDER_SCENE},
    {Resource::AssetType::MODEL, ASSETS_FOLDER_MODEL},
    {Resource::AssetType::TEXTURE, ASSETS_FOLDER_TEXTURE},
    {Resource::AssetType::VIDEO, ASSETS_FOLDER_VIDEO},
    {Resource::AssetType::MATERIAL, ASSETS_FOLDER_MATERIAL},
    {Resource::AssetType::SKYBOX, ASSETS_FOLDER_SKYBOX},
    {Resource::AssetType::FONT, ASSETS_FOLDER_FONT},
    {Resource::AssetType::PREFAB, ASSETS_FOLDER_PREFAB},
};

const std::map<Resource::Type, std::string> ResourcesPreferences::lib_paths = {
    {Resource::Type::SCENE, LIBRARY_FOLDER_SCENE},
    {Resource::Type::MESH, LIBRARY_FOLDER_MESH},
    {Resource::Type::TEXTURE, LIBRARY_FOLDER_TEXTURE},
    {Resource::Type::VIDEO, LIBRARY_FOLDER_VIDEO},
    {Resource::Type::MATERIAL, LIBRARY_FOLDER_MATERIAL},
    {Resource::Type::ANIMATION, LIBRARY_FOLDER_ANIMATION},
    {Resource::Type::SKYBOX, LIBRARY_FOLDER_SKYBOX},
    {Resource::Type::FONT, LIBRARY_FOLDER_FONT},
    {Resource::Type::NAVMESH, LIBRARY_FOLDER_NAVMESH},
    {Resource::Type::PREFAB, LIBRARY_FOLDER_PREFAB},
};

ResourcesPreferences::ResourcesPreferences()
    : Preferences(Type::RESOURCE_PREFS)
{
    group_name = RESOURCES_NODE;
}

void ResourcesPreferences::LoadConfigurationData(const YAML::Node& node)
{
    scene_name = node[SCENE_NAME].as<std::string>();
    scene_id = node[SCENE_ID].as<UID>();
}

void ResourcesPreferences::SaveConfigurationData(YAML::Node& node)
{
    node[group_name][SCENE_NAME] = scene_name + SCENE_EXTENSION;
    node[group_name][SCENE_ID] = scene_id;
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

const char* ResourcesPreferences::GetLibraryPath(Resource::Type type)
{
    auto it = lib_paths.find(type);
    if (it == lib_paths.end())
    {
        assert(false);
    }
    return it->second.c_str();
}
