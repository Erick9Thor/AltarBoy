#include "core/hepch.h"

#include "ImporterManager.h"

#include "MeshImporter.h"
#include "ModelImporter.h"
#include "TextureImporter.h"
#include "MaterialImporter.h"
#include "SceneImporter.h"

using namespace Hachiko;

ImporterManager::ImporterManager()
{
    const auto mesh = new MeshImporter();
    const auto model = new ModelImporter();
    const auto texture = new TextureImporter();
    const auto material = new MaterialImporter();
    //const auto scene = new SceneImporter();

    importers.reserve((size_t)Importer::Type::COUNT);
    importers.push_back(std::make_pair<Importer::Type, Importer*>(model->GetType(), model));
    importers.push_back(std::make_pair<Importer::Type, Importer*>(mesh->GetType(), mesh));
    importers.push_back(std::make_pair<Importer::Type, Importer*>(texture->GetType(), texture));
    importers.push_back(std::make_pair<Importer::Type, Importer*>(material->GetType(), material));
    //importers.push_back(std::make_pair<Importer::Type, Importer*>(scene->GetType(), scene));
}

ImporterManager::~ImporterManager()
{
    for (auto it : importers)
    {
        delete it.second;
    }
}

void ImporterManager::Import(const std::string& asset_path, const Resource::Type asset_type)
{
    assert(!asset_path.empty(), "Module Import abort - Given an empty asset path");
    GetImporter(asset_type)->Import(asset_path.c_str());
}

Resource* Hachiko::ImporterManager::Load(Resource::Type type, UID resource_id)
{
    return GetImporter(type)->Load(resource_id);
}

Resource* Hachiko::ImporterManager::Load(Resource::Type type, const char* path)
{
    // There are resources like Models & Materials, that can't be found in filesystem by id
    switch (type)
    {
    case Resource::Type::MODEL:
        return static_cast<ModelImporter*>(GetImporter(Resource::Type::MODEL))->Load(path);
    case Resource::Type::MATERIAL:
        return static_cast<MaterialImporter*>(GetImporter(Resource::Type::MATERIAL))->Load(path);
    default:
        assert(false, "Invalid parameters. Can't load that kind of resource by path. Use UID instead.");
    }
}

bool Hachiko::ImporterManager::CheckIfImported(const char* path) const
{
    std::filesystem::path model_name(path);
    model_name = model_name.filename().replace_extension();
    std::string fullPath = App->preferences->GetResourcesPreference()->GetLibraryPath(Resource::Type::MODEL);
    fullPath.append(model_name.string());

    return std::filesystem::exists(fullPath);
}

Importer* Hachiko::ImporterManager::GetImporter(Resource::Type type)
{
    auto it = std::find_if(importers.begin(), importers.end(), [&](const std::pair<Importer::Type, Importer*>& element)
        { 
            return element.first == ToImporterType(type);
        });

    assert(it != importers.end());
    return it->second;
}

Importer::Type ImporterManager::ToImporterType(const Resource::Type type)
{
    Importer::Type iType;
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

    case Resource::Type::SCENE:
        iType = Importer::Type::SCENE;
        break;
    case Resource::Type::AUDIO:
    case Resource::Type::VIDEO:
    case Resource::Type::SCRIPT:
    case Resource::Type::UNKNOWN:
        assert(false, "Unhandled resource type");
    }

    return iType;
}