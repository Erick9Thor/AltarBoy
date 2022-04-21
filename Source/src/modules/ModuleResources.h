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
        
        // TODO: Replace getters for only one get function
        ResourceModel* GetModel(const std::string& name);
        ResourceMesh* GetMesh(const UID uid, const std::string& model_path = std::string(), int mesh_index = -1);
        ResourceMaterial* GetMaterial(const std::string& material_name);
        ResourceTexture* GetTexture(const std::string& texture_name, const std::string& asset_path = std::string());

        Resource* GetResource(UID uid)  const
        {
            auto it = loaded_resources.find(uid);
            if (it  != loaded_resources.end())
            {
                return it->second;
            }

            return nullptr;
        }

        void CreateResource(Resource::Type type, const std::string& name) const;
        void AssetsLibraryCheck();

    private:
        // TODO: Use only loaded_resources map
        std::map<std::string, ResourceModel*> models;
        std::map<std::string, ResourceMaterial*> materials;
        std::map<std::string, ResourceTexture*> textures;
        std::map<UID, ResourceMesh*> meshes;

        std::map<UID, Resource*> loaded_resources;

        std::vector<std::pair<Hachiko::Resource::Type, std::string>> supported_extensions = 
        {{Hachiko::Resource::Type::TEXTURE, ".png"},
        {Hachiko::Resource::Type::TEXTURE, ".tif"},
        {Hachiko::Resource::Type::MODEL, ".fbx"},
        {Hachiko::Resource::Type::SCENE, SCENE_EXTENSION},
        // Imported Resources
        {Hachiko::Resource::Type::MODEL, MODEL_EXTENSION},
        {Hachiko::Resource::Type::MATERIAL, MATERIAL_EXTENSION}};
        
        Hachiko::ResourcesPreferences* preferences = nullptr;
        Hachiko::ImporterManager importer_manager;
        std::filesystem::path last_resource_path;

        void ImportResource(const std::filesystem::path& asset, Hachiko::Resource::Type asset_type);
        void HandleResource(const std::filesystem::path& path);

        void IterateFolder(const PathNode& folder);
    };
}
