#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <map>

#include "Module.h"
#include "Resources/Resource.h"
#include "core/preferences/src/ResourcesPreferences.h"

namespace Hachiko
{
    class ResourceModel;
    class ResourceMesh;
    class ResourceMaterial;
    class ResourceTexture;

    /* class ModelImporter;
    class MeshImporter;
    class MaterialImporter;
    class TextureImporter;*/

    class ModuleResources : public Module
    {
    private:
        std::vector<std::pair<Hachiko::Resource::Type, std::string>> supported_extensions = {
            {Hachiko::Resource::Type::TEXTURE, ".png"},
            {Hachiko::Resource::Type::TEXTURE, ".tif"},
            {Hachiko::Resource::Type::MODEL, ".fbx"},
            {Hachiko::Resource::Type::SCENE, ".be"}
        };
        Hachiko::ResourcesPreferences* preferences = nullptr;
        std::filesystem::path last_resource_path; // TODO: This will track every resource, his type and path loaded

        void HandleAssetsChanged(const std::filesystem::path& asset, Hachiko::Resource::Type asset_type);
        void HandleResource(const std::filesystem::path& path);

    public:
        bool Init() override;
        bool CleanUp() override;

        [[nodiscard]] std::filesystem::path GetLastResourceLoadedPath() const;
        Hachiko::Resource::Type GetType(const std::filesystem::path& file);

        // TEMPORAL
        
        // Getters
        ResourceModel* GetModel(std::string name) const
        {
            auto it = models.find(name);
            if (it != models.end())
            {
                return it->second;
            }
            return nullptr;
        };
        ResourceMesh* GetMesh(UID uid) const
        {
            auto it = meshes.find(uid);
            if (it != meshes.end())
            {
                return it->second;
            }
            return nullptr;
        };
        ResourceMaterial* GetMaterial(UID uid) const
        {
            auto it = materials.find(uid);
            if (it != materials.end())
            {
                return it->second;
            }
            return nullptr;
        };
        ResourceTexture* GetTexture(UID uid) const
        {
            auto it = textures.find(uid);
            if (it != textures.end())
            {
                return it->second;
            }
            return nullptr;
        };
        
        std::map<std::string, ResourceModel*> models;
        std::map<UID, ResourceMesh*> meshes;
        std::map<UID, ResourceMaterial*> materials;
        std::map<UID, ResourceTexture*> textures;
       
        /* ModelImporter model_importer;
        MeshImporter mesh_importer;
        MaterialImporter material_importer;
        TextureImporter texture_importer; */
    };
}
