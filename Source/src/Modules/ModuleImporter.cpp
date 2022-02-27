#include "core/hepch.h"

#include "ModuleImporter.h"
#include "ModuleEvent.h"

#include "importers/MeshImporter.h"
#include "importers/ModelImporter.h"
#include "importers/TextureImporter.h"
#include "importers/SceneImporter.h"

using namespace Hachiko;

ModuleImporter::ModuleImporter()
{
    //const auto mesh = new MeshImporter();
    const auto model = new ModelImporter();
    //const auto texture = new TextureImporter();
    //const auto scene = new SceneImporter();

    importers.reserve((size_t)Importer::Type::COUNT);
    importers.push_back(std::make_pair<Importer::Type, Importer*>(model->GetType(), model));
    //importers.push_back(std::make_pair<Importer::Type, Importer*>(mesh->GetType(), mesh));
    //importers.push_back(std::make_pair<Importer::Type, Importer*>(texture->GetType(), texture));
    //importers.push_back(std::make_pair<Importer::Type, Importer*>(scene->GetType(), scene));
}

bool ModuleImporter::Init()
{
    std::function handleNewAsset = [&](Event& evt)
    {
        const auto& e = evt.GetEventData<AssetsAddedEventPayload>();
        HE_LOG("Handling asset: %s, of type: %d", e.GetPath().string().c_str(), e.GetType());
        //Running import in a thread
        std::thread import_thread([=]() { ImportAsset(e.GetPath().string(), e.GetType()); });
        import_thread.detach();
    };
    App->event->Subscribe(Event::Type::ASSETS_CHANGED, handleNewAsset);
    return true;
}

bool ModuleImporter::CleanUp()
{
    for (auto it : importers)
    {
        delete it.second;
    }

    return true;
}

bool ModuleImporter::RestoreLibrary()
{
    return false;
}

void ModuleImporter::ImportAsset(const std::string& asset_path, const Resource::Type asset_type)
{
    assert(!asset_path.empty(), "Module Import abort - Given an empty asset path");
    auto it = std::find_if(importers.begin(), importers.end(),
                           [&](const std::pair<Importer::Type, Importer*>& element)
                           {
                               return element.first == ToImporterType(asset_type);
                           });

    it->second->Import(asset_path.c_str());
}

Importer::Type ModuleImporter::ToImporterType(const Resource::Type type)
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