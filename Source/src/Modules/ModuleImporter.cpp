#include "core/bepch.h"

#include "ModuleImporter.h"

#include "importers/GenericImporter.h"
#include "importers/MeshImporter.h"
#include "importers/ModelImporter.h"
#include "importers/TextureImporter.h"
#include "importers/SceneImporter.h"

using namespace BoxerEngine;

ModuleImporter::ModuleImporter()
{
    const auto gen = new GenericImporter();
    const auto mesh = new MeshImporter();
    const auto model = new ModelImporter();
    const auto texture = new TextureImporter();
    const auto scene = new SceneImporter();

    importers.push_back(std::make_pair<Importer::Type, Importer*>(gen->GetType(), gen));
    importers.push_back(std::make_pair<Importer::Type, Importer*>(model->GetType(), model));
    importers.push_back(std::make_pair<Importer::Type, Importer*>(mesh->GetType(), mesh));
    importers.push_back(std::make_pair<Importer::Type, Importer*>(texture->GetType(), texture));
    importers.push_back(std::make_pair<Importer::Type, Importer*>(scene->GetType(), scene));
}

bool ModuleImporter::Init()
{
    preferences = static_cast<ResourcesPreferences*>(App->preferences->GetPreferenceDataByType(Preferences::Type::RESOURCES));

    std::function handleNewAsset = [&](Event& evt)
    {
        const auto& e = evt.GetEventData<AssetsAddedEventPayload>();
        std::filesystem::path file = e.GetPath();
        BE_LOG("Handling asset: %s, of type: %d", file.string().c_str(), e.GetType());

        //Running import in a thread
        std::thread import_thread(&ModuleImporter::ImportAsset, this, file, e.GetType());
        import_thread.detach();
    };
    EventManager::GetInstance().Subscribe(Event::Type::ASSETS_CHANGED, handleNewAsset);
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

void ModuleImporter::ImportAsset(const std::filesystem::path& asset_path, const ResourceType asset_type)
{
    auto it = std::find_if(importers.begin(), importers.end(),
                           [&](const std::pair<Importer::Type, Importer*>& element)
                           {
                               return element.first == ToImporterType(asset_type);
                           });

    it->second->ImportAsset(asset_path);
}

Importer::Type ModuleImporter::ToImporterType(const ResourceType type)
{
    Importer::Type iType;
    switch (type)
    {
    case ResourceType::MODEL:
        iType = Importer::Type::MODEL;
        break;

    case ResourceType::MESH:
        iType = Importer::Type::MESH;
        break;

    case ResourceType::TEXTURE:
        iType = Importer::Type::TEXTURE;
        break;

    case ResourceType::SCENE:
        iType = Importer::Type::SCENE;
        break;
    case ResourceType::AUDIO:
    case ResourceType::VIDEO:
    case ResourceType::SCRIPT:
    case ResourceType::UNKNOWN:
        iType = Importer::Type::GENERIC;
    }

    return iType;
}
