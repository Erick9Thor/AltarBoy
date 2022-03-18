#include "core/hepch.h"

#include "ModuleResources.h"
#include "Application.h"
#include "ModuleEvent.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneManager.h"

#include "Core/preferences/src/ResourcesPreferences.h"

#include "importers/MeshImporter.h"
#include "importers/TextureImporter.h"
#include "importers/MaterialImporter.h"
#include "importers/ModelImporter.h"
#include "components/ComponentMesh.h"

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

    // Load all resources in library 
    std::vector<std::string> files;
    std::vector<std::string> directories;

    // Load meshes
    ModuleFileSystem::DiscoverFiles(LIBRARY_MESH_FOLDER, files, directories);
    MeshImporter mesh_importer;
    // TODO: LOAD MESH
    files.clear();
    directories.clear();

    // Load textures
    ModuleFileSystem::DiscoverFiles(LIBRARY_TEXTURES_FOLDER, files, directories);
    TextureImporter texture_importer;
    // TODO: LOAD TEXTURE
    files.clear();
    directories.clear();

    // Load materials
    ModuleFileSystem::DiscoverFiles(LIBRARY_MATERIAL_FOLDER, files, directories);
    MaterialImporter material_importer;
    // TODO: LOAD MATERIAL
    files.clear();
    directories.clear();
    
    // Load models
    ModuleFileSystem::DiscoverFiles(LIBRARY_MODEL_FOLDER, files, directories);
    ModelImporter model_importer;
    // TODO: LOAD MODEL
    files.clear();
    directories.clear();

    return true;
}

bool ModuleResources::CleanUp()
{
    for (auto it = meshes.begin(); it != meshes.end(); ++it)
        delete it->second;

    for (auto it = textures.begin(); it != textures.end(); ++it)
        delete it->second;

    for (auto it = materials.begin(); it != materials.end(); ++it)
        delete it->second;

    for (auto it = models.begin(); it != models.end(); ++it)
        delete it->second;

    return Module::CleanUp();
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
        HandleAssetsChanged(destination, type);
        HE_LOG("File destination: %s", destination.string().c_str());
    }
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

void Hachiko::ModuleResources::LoadResourceIntoScene(const char* path)
{
    // TODO: This has to check if the resource is loaded and send it to the scene manager
    // TODO: If we call many times/places this function, consideran handling by an event
    std::filesystem::path resource_path(path);
    Resource::Type type = GetType(path);

    // Do this for models
    GameObject* root_game_object = App->scene_manager->GetActiveScene()->CreateNewGameObject(nullptr, resource_path.filename().string().c_str());

    YAML::Node model_node = YAML::LoadFile(path);
    for (int i = 0; i < model_node[NODE_ROOT][NODE_CHILD].size(); ++i)
    {
        GameObject* mesh_game_object = App->scene_manager->GetActiveScene()->CreateNewGameObject(root_game_object, model_node[NODE_ROOT][NODE_CHILD][i][NODE_NAME].as<std::string>().c_str());
        ComponentMesh* component = static_cast<ComponentMesh*>(mesh_game_object->CreateComponent(Component::Type::MESH));
        component->SetID(model_node[NODE_ROOT][NODE_CHILD][i][NODE_MESH_ID].as<UID>());
        component->LoadMesh(model_node[NODE_ROOT][NODE_CHILD][i][NODE_MESH_ID].as<UID>());
    }
}

void ModuleResources::HandleAssetsChanged(const std::filesystem::path& asset_path, const Resource::Type asset_type)
{
    Event assetChanged(Event::Type::ASSETS_CHANGED);
    assetChanged.SetEventData<AssetsAddedEventPayload>(asset_path, asset_type);
    App->event->Publish(assetChanged);
}
