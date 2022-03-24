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
#include "components/ComponentMaterial.h"
#include "resources/ResourceModel.h"
#include "resources/ResourceMaterial.h"
#include "resources/ResourceTexture.h"

using namespace Hachiko;

bool ModuleResources::Init()
{
    preferences = App->preferences->GetResourcesPreference();

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
    for (auto& it : models)
    {
        delete it.second;
    }

    for (auto& it : meshes)
    {
        delete it.second;
    }

    for (auto& it : materials)
    {
        delete it.second;
    }

    for (auto& it : textures)
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

    auto res = static_cast<ResourceMaterial*>(importer_manager.Load(Resource::Type::MATERIAL, uid));
    materials.emplace(uid, res);

    return res;
}

ResourceTexture* Hachiko::ModuleResources::GetTexture(UID uid)
{
    auto it = textures.find(uid);
    if (it != textures.end())
    {
        return it->second;
    }

    auto res = static_cast<ResourceTexture*>(importer_manager.Load(Resource::Type::TEXTURE, uid));
    textures.emplace(uid, res);

    return res;
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
