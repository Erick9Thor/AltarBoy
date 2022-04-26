#pragma once

#include "resources/Resource.h"
#include "importers/ImporterManager.h"
#include "core/preferences/src/ResourcesPreferences.h"

namespace Hachiko
{
    class ResourceModel;
    class ResourceMesh;
    class ResourceMaterial;
    class ResourceTexture;
    class GameObject;

    class ModuleResources : public Module
    {

    public:
        bool Init() override;
        bool CleanUp() override;

        [[nodiscard]] std::filesystem::path GetLastResourceLoadedPath() const;
        Hachiko::Resource::Type GetType(const std::filesystem::path& file);

        Resource* GetResource(Resource::Type type, UID id);
        void CreateResource(Resource::Type type, const std::string& name) const;
        void AssetsLibraryCheck();
        void HandleResource(const std::filesystem::path& path);

    private:
        // TODO: Use only loaded_resources map
        std::map<std::string, ResourceModel*> models;
        std::map<std::string, ResourceMaterial*> materials;
        std::map<std::string, ResourceTexture*> textures;
        std::map<UID, ResourceMesh*> meshes;

        std::map<UID, Resource*> loaded_resources;

        std::vector<std::pair<Hachiko::Resource::Type, std::string>> supported_extensions = 
        {
            {Hachiko::Resource::Type::TEXTURE, ".png"},
            {Hachiko::Resource::Type::TEXTURE, ".jpg"},
            {Hachiko::Resource::Type::TEXTURE, ".tif"},
            {Hachiko::Resource::Type::MODEL, ".fbx"},
            {Hachiko::Resource::Type::SCENE, SCENE_EXTENSION},
            {Hachiko::Resource::Type::MATERIAL, MATERIAL_EXTENSION}
        };
        
        Hachiko::ResourcesPreferences* preferences = nullptr;
        Hachiko::ImporterManager importer_manager;
        std::filesystem::path last_resource_path;

        void ImportResource(const std::filesystem::path& asset, Hachiko::Resource::Type asset_type);
        void GenerateLibrary(const PathNode& folder);
    };
}
