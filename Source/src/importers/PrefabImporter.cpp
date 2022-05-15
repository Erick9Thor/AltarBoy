#include "core/hepch.h"

#include "PrefabImporter.h"
#include "resources/ResourcePrefab.h"
#include "modules/ModuleResources.h"

void Hachiko::PrefabImporter::Import(const char* path, YAML::Node& meta)
{
    // TODO
}

void Hachiko::PrefabImporter::Save(UID id, const Resource* prefab_data)
{
    const std::string resource_path = GetResourcePath(Resource::Type::PREFAB, id);
    auto resource = static_cast<const ResourcePrefab*>(prefab_data);
    const std::string prefab_asset_path = StringUtils::Concat(GetResourcesPreferences()->GetAssetsPath(Resource::AssetType::PREFAB), resource->name, PREFAB_EXTENSION);
    FileSystem::Save(resource_path.c_str(), resource->prefab_data);
    FileSystem::Save(prefab_asset_path.c_str(), resource->prefab_data);
}

Hachiko::Resource* Hachiko::PrefabImporter::Load(UID id)
{
   // TODO
}

Hachiko::UID Hachiko::PrefabImporter::CreatePrefabAsset(const char* name, GameObject* root)
{
    // This id wont be used
    ResourcePrefab* prefab_data = new ResourcePrefab(0);
    prefab_data->name = name;
    constexpr bool as_prefab = true;
    root->Save(prefab_data->prefab_data, as_prefab);

    const std::string prefab_asset_path = StringUtils::Concat(GetResourcesPreferences()->GetAssetsPath(Resource::AssetType::PREFAB), prefab_data->name, PREFAB_EXTENSION);
    FileSystem::Save(prefab_asset_path.c_str(), prefab_data->prefab_data);
    UID prefab_uid = App->resources->HandleAssetFromAnyPath(prefab_asset_path)[0];
    return prefab_uid;
}