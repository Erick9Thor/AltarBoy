#pragma once

#include "Resources/Resource.h"
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
        
        ResourceModel* GetModel(const std::string& name);
        ResourceMesh* GetMesh(const UID uid);

        ResourceMaterial* GetMaterial(UID uid);
        ResourceTexture* GetTexture(UID uid);
        
        void LoadResourceIntoScene(const char* path);
        Resource* GetResource(Resource::Type type, UID resource_id);

        std::map<std::string, ResourceModel*> models;
        std::map<UID, ResourceMesh*> meshes;
        std::map<UID, ResourceMaterial*> materials;
        std::map<UID, ResourceTexture*> textures;

    private:
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
        std::filesystem::path last_resource_path; // TODO: This will track every resource, his type and path loaded

        void ImportResource(const std::filesystem::path& asset, Hachiko::Resource::Type asset_type);
        void HandleResource(const std::filesystem::path& path);
        void LoadModelIntoGameObject(const char* path, GameObject* game_object);
        void LoadMaterialIntoGameObject(const char* path, GameObject* game_object);
    };
}
