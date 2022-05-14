#include "core/hepch.h"

#include "ImporterManager.h"

#include "MeshImporter.h"
#include "ModelImporter.h"
#include "TextureImporter.h"
#include "MaterialImporter.h"
#include "AnimationImporter.h"
#include "FontImporter.h"

using namespace Hachiko;

ImporterManager::ImporterManager()
{
    
    const auto model = new ModelImporter();
    const auto texture = new TextureImporter();
    const auto material = new MaterialImporter();
    const auto font = new FontImporter();

    importers.emplace(Resource::AssetType::MODEL, model);
    importers.emplace(Resource::AssetType::TEXTURE, texture);
    importers.emplace(Resource::AssetType::MATERIAL, material);
    importers.emplace(Resource::AssetType::FONT, font);

    const auto animation = new AnimationImporter();
    const auto mesh = new MeshImporter();

    resource_importers.emplace(Resource::Type::MODEL, model);
    resource_importers.emplace(Resource::Type::TEXTURE, texture);
    resource_importers.emplace(Resource::Type::MATERIAL, material);
    resource_importers.emplace(Resource::Type::FONT, font);
    resource_importers.emplace(Resource::Type::ANIMATION, animation);
    resource_importers.emplace(Resource::Type::MESH, mesh);

}

ImporterManager::~ImporterManager()
{
    for (auto it : importers)
    {
        delete it.second;
    }
}

void ImporterManager::ImportAsset(const std::filesystem::path& asset_path, const Resource::AssetType asset_type, YAML::Node& meta)
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
    auto it = importers.find(type);
    if (it != importers.end())
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

