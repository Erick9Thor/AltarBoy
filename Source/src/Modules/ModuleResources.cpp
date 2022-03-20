#include "core/hepch.h"

#include "ModuleResources.h"
#include "ModuleEvent.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneManager.h"
#include "ModuleEditor.h"

#include "Core/preferences/src/ResourcesPreferences.h"

#include "importers/MeshImporter.h"
#include "importers/TextureImporter.h"
#include "importers/MaterialImporter.h"
#include "importers/ModelImporter.h"

#include "components/ComponentMesh.h"
#include "resources/ResourceModel.h"

using namespace Hachiko;

bool ModuleResources::Init()
{
    preferences = static_cast<ResourcesPreferences*>(App->preferences->GetPreference(Preferences::Type::RESOURCES));

    // create assets & library directory tree
    for (int i = 0; i < static_cast<int>(Resource::Type::UNKNOWN); ++i)
    {
        App->file_sys->CreateDir(preferences->GetLibraryPath(static_cast<Resource::Type>(i)));
        App->file_sys->CreateDir(preferences->GetAssetsPath(static_cast<Resource::Type>(i)));
    }

    std::function handleAddedFile = [&](Event& evt)
    {
        const auto& e = evt.GetEventData<FileAddedEventPayload>();
        std::filesystem::path file = e.GetPath();
        HE_LOG("Handling dropped file: %s", file.string().c_str());
        HandleResource(file);
    };
    App->event->Subscribe(Event::Type::FILE_ADDED, handleAddedFile);
    return true;
}

bool ModuleResources::CleanUp()
{
    for (auto& it : meshes)
    {
        delete it.second;
    }

    for (auto& it : textures)
    {
        delete it.second;
    }

    for (auto& it : materials)
    {
        delete it.second;
    }

    return true;
}

std::filesystem::path ModuleResources::GetLastResourceLoadedPath() const
{
    return last_resource_path;
}

void ModuleResources::HandleResource(const std::filesystem::path& path)
{
    Resource::Type type = GetType(path);
    if (type == Resource::Type::UNKNOWN)
    {
        HE_LOG("Unknown resource type recevied, nothing to be done");
        return;
    }
    std::filesystem::path destination = preferences->GetAssetsPath(type);

    if(App->file_sys->Copy(path.string().c_str(),
        destination.append(path.filename().c_str()).string().c_str(), true))
    {
        last_resource_path = path; // We may need this to import more assets from this path
        ImportResource(destination, type);
        HE_LOG("File destination: %s", destination.string().c_str());
    }
}

void Hachiko::ModuleResources::LoadModelIntoGameObject(const char* path, GameObject* game_object)
{
    YAML::Node model_node = YAML::LoadFile(path);
    for (int i = 0; i < model_node[NODE_ROOT][NODE_CHILD].size(); ++i)
    {
        UID mesh_id = model_node[NODE_ROOT][NODE_CHILD][i][NODE_MESH_ID].as<UID>(); // TODO: Add recursivity
        GameObject* mesh_game_object = App->scene_manager->GetActiveScene()->CreateNewGameObject(game_object, model_node[NODE_ROOT][NODE_CHILD][i][NODE_NAME].as<std::string>().c_str());
        ComponentMesh* component = static_cast<ComponentMesh*>(mesh_game_object->CreateComponent(Component::Type::MESH));
        component->SetID(mesh_id);
        component->AddResourceMesh(GetMesh(mesh_id));
        // We are storing materials by id, we need to change by name so we can call this function from here
        //LoadMaterialIntoGameObject(model_node[MODEL_MATERIAL_NODE], mesh_game_object);
    }
}

void Hachiko::ModuleResources::LoadMaterialIntoGameObject(const char* path, GameObject* game_object)
{
    // GetMaterial(path); // This should retrieve ResourceMaterial* - I've modified getters so they import the resource if is not loaded into map yet. Check GetMesh & GetModel functions
    // ComponentMaterial* component = game_object->CreateComponent(Component::Type::MATERIAL)
    // Set component data...
}
Resource::Type ModuleResources::GetType(const std::filesystem::path& path)
{
    if (!path.has_extension())
    {
        return Resource::Type::UNKNOWN;
    }
    const std::filesystem::path extension = path.extension();

    auto isValidExtension = [&](const std::pair<Resource::Type, std::string>& element)
    {
        return element.second.size() == extension.string().size() &&
            std::equal(element.second.begin(), element.second.end(), extension.string().begin(),
                       [](auto a, auto b) { return std::tolower(a) == std::tolower(b); });
    };

    const auto it = std::find_if(supported_extensions.begin(), supported_extensions.end(), isValidExtension);
    if (it != supported_extensions.end())
    {
        return it->first;
    }
    return Resource::Type::UNKNOWN;
}

ResourceModel* Hachiko::ModuleResources::GetModel(const std::string& name)
{
    auto it = models.find(name);
    if (it != models.end())
    {
        return it->second;
    }

    auto res = static_cast<ResourceModel*>(importer_manager.Load(Resource::Type::MODEL, name.c_str()));
    models.emplace(name, res);

    return res;
}

ResourceMesh* Hachiko::ModuleResources::GetMesh(const UID uid)
{
    auto it = meshes.find(uid);
    if (it != meshes.end())
    {
        return it->second;
    }

    auto res = static_cast<ResourceMesh*>(importer_manager.Load(Resource::Type::MESH, uid));
    meshes.emplace(uid, res);
    
    return res;
}

ResourceMaterial* Hachiko::ModuleResources::GetMaterial(UID uid)
{
    // We need to change this map to get it by name
    auto it = materials.find(uid);
    if (it != materials.end())
    {
        return it->second;
    }

    // auto res = static_cast<ResourceMaterial*>(importer_manager.Load(Resource::Type::MATERIAL, path)); // I've overload Load function in ImporterManager to admit loading by name for specific types
    // material.emplace(name, res);
    // return res;
    return nullptr;
}

void Hachiko::ModuleResources::LoadResourceIntoScene(const char* path)
{
    // TODO: If we call many times/places this function, consider handling by an event
    GameObject* game_object = App->editor->GetSelectedGameObject();
    if (game_object == nullptr)
    {
        std::filesystem::path resource_path(path);
        game_object = App->scene_manager->GetActiveScene()->CreateNewGameObject(nullptr, resource_path.filename().replace_extension().string().c_str());
    }

    Resource::Type type = GetType(path);
    switch (type)
    {
    case Resource::Type::MODEL:
        LoadModelIntoGameObject(path, game_object);
        break;
    case Resource::Type::MATERIAL:
        LoadMaterialIntoGameObject(path, game_object);
        break;
    }
}

Resource* Hachiko::ModuleResources::GetResource(Resource::Type type, UID resource_id)
{
    switch (type)
    {
    case Resource::Type::MESH:
        {
            return GetMesh(resource_id); 
        }
    default:
        return nullptr;
    }
}

void ModuleResources::ImportResource(const std::filesystem::path& asset_path, const Resource::Type asset_type)
{
    importer_manager.Import(asset_path.string(), asset_type);
}
