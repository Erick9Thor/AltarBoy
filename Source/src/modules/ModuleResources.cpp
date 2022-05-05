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
        HandleResource(file);
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

void ModuleResources::HandleResource(const std::filesystem::path& path)
{
    Resource::Type type = GetType(path);
    
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
        HE_LOG("Resource file in assets folder");
        destination = std::filesystem::relative(path, path.string().substr(0, relative_pos));
    }
    else
    {
        HE_LOG("Resource file outside the assets folder");
        destination = preferences->GetAssetsPath(type);
        destination.append(path.filename().c_str());
        FileSystem::Copy(path.string().c_str(), destination.string().c_str(), true);
    }

    // Import asset
    if (!importer_manager.IsImported(path.string().c_str(), type))
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

void Hachiko::ModuleResources::CreateResource(Resource::Type type, const std::string& name) const
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

void Hachiko::ModuleResources::ReimportLibrary()
{
    FileSystem::Delete(LIBRARY_FOLDER);

    AssetsLibraryCheck();

    // RELOAD SCENE
}

void Hachiko::ModuleResources::ReimportAsset(std::string meta_path)
{
    YAML::Node meta_node = YAML::LoadFile(meta_path);

    Resource::Type type = static_cast<Resource::Type>(meta_node[GENERAL_NODE][GENERAL_TYPE].as<int>());
    std::string asset_path = meta_path.substr(0, meta_path.length() - 5);

    importer_manager.DeleteWithMeta(type, meta_node);
    importer_manager.ImportWithMeta(std::filesystem::path(asset_path), type, meta_node);

    // RELOAD SCENE
}

// A WAY TO REMOVE ALL METAS
void DeleteMetas(const PathNode& folder)
{
    for (PathNode path_node : folder.children)
    {
        if (path_node.isFile)
        {
            FileSystem::Delete(path_node.path.c_str());
        }
        else
        {
            DeleteMetas(path_node);
        }
    }
}
//

void Hachiko::ModuleResources::AssetsLibraryCheck()
{
    HE_LOG("Assets/Library check...");

    // CAREFULL! A WAY TO REMOVE ALL METAS
    //std::vector<std::string> meta_extension {"meta"};
    //PathNode metas = FileSystem::GetAllFiles(ASSETS_FOLDER, &meta_extension, nullptr);
    //DeleteMetas(metas);
    //

    // TODO: use defined values, for extensions and assets
    std::vector<std::string> meta_ext {"meta", "scene"};
    PathNode assets_folder = FileSystem::GetAllFiles(ASSETS_FOLDER, nullptr, &meta_ext);
    //PathNode assets_folder = FileSystem::GetAllFiles("assets", &meta_ext, nullptr); // TODO: check that all meta has its asset
    // TODO: check library folder

    GenerateLibrary(assets_folder); // TODO: Add preference for (de)activate this check

    HE_LOG("Assets/Library check finished.");
}

void Hachiko::ModuleResources::GenerateLibrary(const PathNode& folder) 
{
    for (PathNode path_node : folder.children)
    {
        if (path_node.isFile)
        {
            Resource::Type type = GetType(path_node.path);
            
            if (type == Resource::Type::UNKNOWN)
            {
                continue;
            }

            std::string meta_path = StringUtils::Concat(path_node.path, META_EXTENSION);

            if (FileSystem::Exists(meta_path.c_str()))
            {
                YAML::Node meta_node = YAML::LoadFile(meta_path);

                // Extract data from meta
                UID meta_uid = meta_node[GENERAL_NODE][GENERAL_ID].as<UID>();
                uint64_t meta_hash = meta_node[GENERAL_NODE][GENERAL_HASH].IsDefined() ? meta_node[GENERAL_NODE][GENERAL_HASH].as<uint64_t>() : 0;
                
                std::string library_path = StringUtils::Concat(preferences->GetLibraryPath(type), std::to_string(meta_uid));
                bool library_file_exists = FileSystem::Exists(library_path.c_str());

                // Get the asset timestamp
                uint64_t asset_hash = FileSystem::HashFromPath(path_node.path.c_str());

                if (meta_hash != asset_hash)
                {
                    if (library_file_exists)
                    {
                        importer_manager.DeleteWithMeta(type, meta_node);
                    }
                    importer_manager.ImportWithMeta(std::filesystem::path(path_node.path), type, meta_node);
                }
                else if (!library_file_exists)
                {
                    importer_manager.ImportWithMeta(std::filesystem::path(path_node.path), type, meta_node);
                }
            }
            else
            {
                importer_manager.Import(std::filesystem::path(path_node.path), type);
            }
        }
        else
        {
            GenerateLibrary(path_node);
        }
    }
}

void ModuleResources::ImportResource(const std::filesystem::path& asset_path, const Resource::Type asset_type)
{
    importer_manager.Import(asset_path.string(), asset_type);
}