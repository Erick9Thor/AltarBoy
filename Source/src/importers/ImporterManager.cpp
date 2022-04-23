#include "core/hepch.h"

#include "ImporterManager.h"

#include "MeshImporter.h"
#include "ModelImporter.h"
#include "TextureImporter.h"
#include "MaterialImporter.h"
#include "AnimationImporter.h"

using namespace Hachiko;

ImporterManager::ImporterManager()
{
    const auto mesh = new MeshImporter();
    const auto model = new ModelImporter();
    const auto texture = new TextureImporter();
    const auto material = new MaterialImporter();
    const auto animation = new AnimationImporter();

    importers.emplace(model->GetType(), model);
    importers.emplace(mesh->GetType(), mesh);
    importers.emplace(texture->GetType(), texture);
    importers.emplace(material->GetType(), material);
    importers.emplace(animation->GetType(), animation);
}

ImporterManager::~ImporterManager()
{
    for (auto it : importers)
    {
        delete it.second;
    }
}

void ImporterManager::Import(const std::filesystem::path& asset_path, const Resource::Type asset_type)
{
    assert(!asset_path.empty() && "Module Import abort - Given an empty asset path");
    
    YAML::Node meta = CreateMeta(asset_path.string().c_str(), asset_type);
    
    GetImporter(asset_type)->Import(asset_path.string().c_str(), meta);

    std::string meta_path = StringUtils::Concat(asset_path.parent_path().string(), "\\", asset_path.filename().string(), META_EXTENSION);
    FileSystem::Save(meta_path.c_str(), meta);
}

Resource* Hachiko::ImporterManager::Load(Resource::Type type, UID id)
{
    return GetImporter(type)->Load(id);
}

bool Hachiko::ImporterManager::IsImported(const char* path, Resource::Type type) const
{
    return GetImporter(type)->IsImported(path);
}

void Hachiko::ImporterManager::ImportWithMeta(const std::filesystem::path& asset_path, Resource::Type asset_type, YAML::Node& meta) const 
{
    assert(!asset_path.empty() && "Module Import abort - Given an empty asset path");

    UpdateMeta(asset_path.string().c_str(), asset_type, meta);

    GetImporter(asset_type)->ImportWithMeta(asset_path.string().c_str(), meta);

    std::string meta_path = StringUtils::Concat(asset_path.parent_path().string(), "\\", asset_path.filename().string(), META_EXTENSION);
    FileSystem::Save(meta_path.c_str(), meta);
}

void Hachiko::ImporterManager::DeleteWithMeta(Resource::Type asset_type, const YAML::Node& meta) const 
{
    GetImporter(asset_type)->Delete(meta);
}

Importer* Hachiko::ImporterManager::GetImporter(Resource::Type type) const
{
    auto it = importers.find(ToImporterType(type));
    return it->second;
}

Importer::Type ImporterManager::ToImporterType(const Resource::Type type) const
{
    Importer::Type iType = Importer::Type::COUNT;
    switch (type)
    {
    case Resource::Type::MODEL:
        iType = Importer::Type::MODEL;
        break;

    case Resource::Type::MESH:
        iType = Importer::Type::MESH;
        break;

    case Resource::Type::TEXTURE:
        iType = Importer::Type::TEXTURE;
        break;

    case Resource::Type::MATERIAL:
        iType = Importer::Type::MATERIAL;
        break;
    case Resource::Type::ANIMATION:
        iType = Importer::Type::ANIMATION;
        break;
    case Resource::Type::SCENE:
        iType = Importer::Type::SCENE;
        break;
    case Resource::Type::AUDIO:
    case Resource::Type::VIDEO:
    case Resource::Type::SCRIPT:
    case Resource::Type::UNKNOWN:
        // TODO: This is a hack. We need to implement our own assert with message
        assert(false && "Unhandled resource type");
    }

    return iType;
}

YAML::Node Hachiko::ImporterManager::CreateMeta(const char* path, const Resource::Type resource_type) const
{
    YAML::Node node;
    std::string file_path(path);
    auto last_time_write = GetFileLastWriteTime(StringUtils::StringToWString(file_path));

    node[GENERAL_NODE][GENERAL_ID] = UUID::GenerateUID();
    node[GENERAL_NODE][GENERAL_FILE_PATH] = file_path;
    node[GENERAL_NODE][GENERAL_TYPE] = static_cast<int>(resource_type);
    node[GENERAL_NODE][GENERAL_LAST_WRITE_TIME] = last_time_write;
    
    return node;
}

void Hachiko::ImporterManager::UpdateMeta(const char* path, const Resource::Type resource_type, YAML::Node& meta) const
{
    std::string file_path(path);
    auto last_time_write = GetFileLastWriteTime(StringUtils::StringToWString(file_path));

    meta[GENERAL_NODE][GENERAL_TYPE] = static_cast<int>(resource_type);
    meta[GENERAL_NODE][GENERAL_LAST_WRITE_TIME] = last_time_write;
}
