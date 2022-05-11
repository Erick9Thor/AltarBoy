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

    importers.emplace(model->GetType(), model);
    importers.emplace(mesh->GetType(), mesh);
    importers.emplace(texture->GetType(), texture);
    importers.emplace(material->GetType(), material);
    importers.emplace(animation->GetType(), animation);
    importers.emplace(font->GetType(), font);
}

ImporterManager::~ImporterManager()
{
    for (auto it : importers)
    {
        delete it.second;
    }
}

void ImporterManager::Import(const std::filesystem::path& asset_path, const Resource::Type asset_type, UID id)
{
    assert(!asset_path.empty() && "Module Import abort - Given an empty asset path");
    
    // If the id passed is 0 (default) a new id is generated
    YAML::Node meta = CreateMeta(asset_path.string().c_str(), asset_type, id);   
    
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
    auto it = importers.find(ToImporterType(type));
    return it->second;
}

Importer::Type ImporterManager::ToImporterType(const Resource::Type type) const
{
    Importer::Type importer_type = Importer::Type::COUNT;
    switch (type)
    {
    case Resource::Type::MODEL:
        importer_type = Importer::Type::MODEL;
        break;

    case Resource::Type::MESH:
        importer_type = Importer::Type::MESH;
        break;

    case Resource::Type::TEXTURE:
        importer_type = Importer::Type::TEXTURE;
        break;

    case Resource::Type::MATERIAL:
        importer_type = Importer::Type::MATERIAL;
        break;
    case Resource::Type::ANIMATION:
        importer_type = Importer::Type::ANIMATION;
        break;
    case Resource::Type::SCENE:
        importer_type = Importer::Type::SCENE;
        break;
    case Resource::Type::FONT:
        importer_type = Importer::Type::FONT;
        break;
    case Resource::Type::AUDIO:
    case Resource::Type::VIDEO:
    case Resource::Type::SCRIPT:
    case Resource::Type::UNKNOWN:
    default:
        // TODO: This is a hack. We need to implement our own assert with message
        assert(false && "Unhandled resource type");
    }

    return importer_type;
}

YAML::Node Hachiko::ImporterManager::CreateMeta(const char* path, const Resource::Type resource_type, UID id) const
{
    YAML::Node node;
    uint64_t file_hash = FileSystem::HashFromPath(path);
    if (id != 0)
    {
        node[GENERAL_NODE][GENERAL_ID] = id;
    }
    else
    {
        node[GENERAL_NODE][GENERAL_ID] = UUID::GenerateUID();
    }    
    node[GENERAL_NODE][GENERAL_TYPE] = static_cast<int>(resource_type);    
    node[GENERAL_NODE][GENERAL_HASH] = file_hash;
    
    return node;
}

void Hachiko::ImporterManager::UpdateMeta(const char* path, YAML::Node& meta) const
{
    uint64_t file_hash = FileSystem::HashFromPath(path);
    meta[GENERAL_NODE][GENERAL_HASH] = file_hash;
}
