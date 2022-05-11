#include "core/hepch.h"

#include "ModuleResources.h"
#include "ModuleEvent.h"
#include "ModuleSceneManager.h"
#include "ModuleEditor.h"

#include "core/preferences/src/ResourcesPreferences.h"

#include "importers/MeshImporter.h"
#include "importers/TextureImporter.h"
#include "importers/MaterialImporter.h"
#include "importers/ModelImporter.h"

#include "components/ComponentMeshRenderer.h"
#include "resources/ResourceModel.h"
#include "resources/ResourceMaterial.h"
#include "resources/ResourceTexture.h"

using namespace Hachiko;

bool ModuleResources::Init()
{
    preferences = App->preferences->GetResourcesPreference();

    // create assets & library directory tree
    for (int i = 1; i < static_cast<int>(Resource::Type::COUNT); ++i)
    {
        FileSystem::CreateDir(preferences->GetLibraryPath(static_cast<Resource::Type>(i)));
        FileSystem::CreateDir(preferences->GetAssetsPath(static_cast<Resource::Type>(i)));
    }

    AssetsLibraryCheck();

    std::function handleAddedFile = [&](Event& evt)
    {
        const auto& e = evt.GetEventData<FileAddedEventPayload>();
        std::filesystem::path file = e.GetPath();
        HE_LOG("Handling dropped file: %s", file.string().c_str());
        HandleAssetFromAnyPath(file);
    };
    App->event->Subscribe(Event::Type::FILE_ADDED, handleAddedFile);
    return true;
}

bool ModuleResources::CleanUp()
{
    for (auto& it : loaded_resources)
    {
        RELEASE(it.second);
    }

    return true;
}

std::filesystem::path ModuleResources::GetLastResourceLoadedPath() const
{
    return last_resource_path.parent_path();
}

void ModuleResources::HandleAssetFromAnyPath(const std::filesystem::path& path)
{
    Resource::Type type = GetTypeFromPath(path);
    
    if (type == Resource::Type::UNKNOWN)
    {
        HE_LOG("Unknown resource type recevied, nothing to be done");
        return;
    }

    size_t relative_pos = path.string().find("assets\\");
    bool file_in_asset = relative_pos != std::string::npos;
    std::filesystem::path destination;

    if (file_in_asset)
    {
        HE_LOG("File in assets folder");
        destination = std::filesystem::relative(path, path.string().substr(0, relative_pos));
    }
    else
    {
        HE_LOG("File outside the assets folder");
        destination = preferences->GetAssetsPath(type);
        destination.append(path.filename().c_str());
        FileSystem::Copy(path.string().c_str(), destination.string().c_str(), true);
    }

    // Handle asset when it is in correct path
    HandleAssetInCorrectPath(destination.string())
}

Resource::Type ModuleResources::GetTypeFromPath(const std::filesystem::path& path)
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

Resource* Hachiko::ModuleResources::GetResource(Resource::Type type, UID id)
{
    auto it = loaded_resources.find(id);
    if (it != loaded_resources.end())
    {
        return it->second;
    }

    auto res = importer_manager.Load(type, id);
    if (res != nullptr)
    {
        return loaded_resources.emplace(id, res).first->second;
    }

    return nullptr;
}

void Hachiko::ModuleResources::CreateAsset(Resource::Type type, const std::string& name) const
{
    switch (type)
    {
    case Resource::Type::MATERIAL:
        {
            MaterialImporter material_importer;
            material_importer.CreateMaterial(name);
        }
    }
}

void Hachiko::ModuleResources::AssetsLibraryCheck()
{
    HE_LOG("Assets/Library check...");

    // Ignores files that dont need any short of importing themselves
    // Metas are searched for based on what's on assets
    std::vector<std::string> ignore_extensions {"meta", "scene"};
    PathNode assets_folder = FileSystem::GetAllFiles(ASSETS_FOLDER, nullptr, &ignore_extensions);
    GenerateLibrary(assets_folder);

    HE_LOG("Assets/Library check finished.");
}

void Hachiko::ModuleResources::GenerateLibrary(const PathNode& folder) 
{
    // Iterate all files found in assets except metas and scene
    for (PathNode path_node : folder.children)
    {
        if (!path_node.isFile)
        {
            GenerateLibrary(path_node);
            continue;
        }
        HandleAssetInCorrectPath(path_node.path);
    }
}

void Hachiko::ModuleResources::HandleAssetInCorrectPath(const std::string& asset_path)
{
    Resource::Type type = GetTypeFromPath(asset_path);

    if (type == Resource::Type::UNKNOWN)
    {
        // Discard unrecognised types
        return;
    }

    // Infer meta path from the asset name
    std::string meta_path = StringUtils::Concat(asset_path, META_EXTENSION);

    if (!FileSystem::Exists(meta_path.c_str()))
    {
        // If it doesnt have meta import from scratch, dont pass uid so it generates a new one
        ImportResourceFromAsset(std::filesystem::path(asset_path), type);
        return;
    }

    // If it has meta align on library and refresh if necessary
    YAML::Node meta_node = YAML::LoadFile(meta_path);

    // Extract data from meta
    UID meta_uid = meta_node[GENERAL_NODE][GENERAL_ID].as<UID>();
    uint64_t previous_asset_hash = meta_node[GENERAL_NODE][GENERAL_HASH].IsDefined() ? meta_node[GENERAL_NODE][GENERAL_HASH].as<uint64_t>() : 0;

    std::string library_path = StringUtils::Concat(preferences->GetLibraryPath(type), std::to_string(meta_uid));
    bool library_file_exists = FileSystem::Exists(library_path.c_str());

    // Get the asset hash and reimport if it has changed or if it is not in lib
    uint64_t asset_hash = FileSystem::HashFromPath(asset_path.c_str());
    if (previous_asset_hash != asset_hash || !library_file_exists)
    {
        // Remove previous lib version if it exists
        if (library_file_exists)
        {
            importer_manager.Delete(meta_uid, type);
        }
        // Pass uid so it is maintained
        ImportResourceFromAsset(std::filesystem::path(asset_path), type, meta_uid);
    }
}

void ModuleResources::ImportResourceFromAsset(const std::filesystem::path& asset_path, const Resource::Type asset_type, UID uid)
{
    // If passed uid is 0 (default) it will generate a new one
    importer_manager.Import(asset_path.string(), asset_type, uid);
}