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
    const auto mesh = new MeshImporter();
    const auto model = new ModelImporter();
    const auto texture = new TextureImporter();
    const auto material = new MaterialImporter();
    const auto animation = new AnimationImporter();
    const auto font = new FontImporter();

    importers.emplace(Resource::Type::MODEL, model);
    importers.emplace(Resource::Type::MESH, mesh);
    importers.emplace(Resource::Type::TEXTURE, texture);
    importers.emplace(Resource::Type::MATERIAL, material);
    importers.emplace(Resource::Type::ANIMATION, animation);
    importers.emplace(Resource::Type::FONT, font);
}

ImporterManager::~ImporterManager()
{
    for (auto it : importers)
    {
        delete it.second;
    }
}

void ImporterManager::Import(const std::filesystem::path& asset_path, const Resource::Type asset_type, YAML::Node& meta)
{
    assert(!asset_path.empty() && "Module Import abort - Given an empty asset path");  

    Importer* importer = GetImporter(asset_type);

    if (!importer)
    {
        HE_LOG("No valid importer found for %s", asset_path.c_str());
        return;
    }

    importer->Import(asset_path.string().c_str(), meta);

    std::string meta_path = StringUtils::Concat(asset_path.parent_path().string(), "\\", asset_path.filename().string(), META_EXTENSION);
    FileSystem::Save(meta_path.c_str(), meta);
}

Resource* Hachiko::ImporterManager::Load(Resource::Type type, UID id)
{
    return GetImporter(type)->Load(id);
}

void Hachiko::ImporterManager::Delete(UID uid, Resource::Type resource_type) const
{
    GetImporter(resource_type)->Delete(uid, resource_type);
}

Importer* Hachiko::ImporterManager::GetImporter(Resource::Type type) const
{
    auto it = importers.find(type);
    return it->second;
}

