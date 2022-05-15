#include "core/hepch.h"

#include "ImporterManager.h"

#include "MeshImporter.h"
#include "ModelImporter.h"
#include "TextureImporter.h"
#include "MaterialImporter.h"
#include "AnimationImporter.h"
#include "FontImporter.h"
#include "PrefabImporter.h"

using namespace Hachiko;

ImporterManager::ImporterManager()
{
    // They exist both as assets and as resources
    const auto material = new MaterialImporter();
    const auto texture = new TextureImporter();    
    const auto font = new FontImporter();
    const auto prefab = new PrefabImporter();

    // It doesnt have its own resource type (we use prefabs)
    const auto model = new ModelImporter();    

    // They dont have their own asset type
    const auto animation = new AnimationImporter();
    const auto mesh = new MeshImporter();

    // Importers used to generate resources from asset types
    asset_importers.emplace(Resource::AssetType::TEXTURE, texture);
    asset_importers.emplace(Resource::AssetType::MATERIAL, material);
    asset_importers.emplace(Resource::AssetType::FONT, font);
    asset_importers.emplace(Resource::AssetType::PREFAB, prefab);
    asset_importers.emplace(Resource::AssetType::MODEL, model);    

    // Importers used to save and load the different resource types
    resource_importers.emplace(Resource::Type::TEXTURE, texture);
    resource_importers.emplace(Resource::Type::MATERIAL, material);
    resource_importers.emplace(Resource::Type::FONT, font);
    resource_importers.emplace(Resource::Type::PREFAB, prefab);
    resource_importers.emplace(Resource::Type::MESH, mesh);
    resource_importers.emplace(Resource::Type::ANIMATION, animation);
}

ImporterManager::~ImporterManager()
{
    for (auto it : asset_importers)
    {
        delete it.second;
    }
}

std::vector<UID> ImporterManager::ImportAsset(const std::filesystem::path& asset_path, const Resource::AssetType asset_type, YAML::Node& meta)
{
    // Find the corresponding importer for an asset, delegate import to it and them store the resulting meta
    Importer* importer = GetAssetImporter(asset_type);

    if (!importer)
    {
        HE_LOG("No valid importer found for %s", asset_path.c_str());
        return;
    }

    importer->Import(asset_path.string().c_str(), meta);

    std::string meta_path = StringUtils::Concat(asset_path.parent_path().string(), "\\", asset_path.filename().string(), META_EXTENSION);
    FileSystem::Save(meta_path.c_str(), meta);
    std::vector<UID> generated_resource_ids;
    for (unsigned i = 0; i < meta[RESOURCES].size(); ++i)
    {
        generated_resource_ids.push_back(meta[RESOURCES][i].as<UID>());
    }
    return generated_resource_ids;
}

Resource* Hachiko::ImporterManager::LoadResource(Resource::Type type, UID id)
{
    return GetResourceImporter(type)->Load(id);
}

void Hachiko::ImporterManager::DeleteResource(UID uid, Resource::Type resource_type) const
{
    GetResourceImporter(resource_type)->Delete(uid, resource_type);
}

Importer* Hachiko::ImporterManager::GetAssetImporter(Resource::AssetType type) const
{
    auto it = asset_importers.find(type);
    if (it != asset_importers.end())
    {
        return it->second;
    }
    return nullptr;
}

Importer* Hachiko::ImporterManager::GetResourceImporter(Resource::Type type) const
{
    auto it = resource_importers.find(type);
    if (it != resource_importers.end())
    {
        return it->second;
    }
    return nullptr;    
}

