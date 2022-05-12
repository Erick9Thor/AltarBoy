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

        // TODO: Remove this should not exist
        [[nodiscard]] std::filesystem::path GetLastResourceLoadedPath() const;


        Hachiko::Resource::Type GetTypeFromPath(const std::filesystem::path& file);

        Resource* GetResource(Resource::Type type, UID id);
        void CreateAsset(Resource::Type type, const std::string& name) const;
        void AssetsLibraryCheck();
        void HandleAssetFromAnyPath(const std::filesystem::path& path);

    private:
        std::map<UID, Resource*> loaded_resources;

        std::vector<std::pair<Hachiko::Resource::Type, std::string>> supported_extensions = 
        {
            {Hachiko::Resource::Type::TEXTURE, ".png"},
            {Hachiko::Resource::Type::TEXTURE, ".jpg"},
            {Hachiko::Resource::Type::TEXTURE, ".tif"},
            {Hachiko::Resource::Type::MODEL, ".fbx"},
            {Hachiko::Resource::Type::SCENE, SCENE_EXTENSION},
            {Hachiko::Resource::Type::MATERIAL, MATERIAL_EXTENSION},
            {Hachiko::Resource::Type::FONT, ".ttf"},
        };
        
        Hachiko::ResourcesPreferences* preferences = nullptr;
        Hachiko::ImporterManager importer_manager;
        std::filesystem::path last_resource_path;

        // Checks the current assets folder states and sets library to a valid state
        void GenerateLibrary(const PathNode& folder);
        // Gets an asset file and returns its related resources
        void ImportAssetResources(const std::string& asset_path);
        // From an asset file creates its resources and sets the corresponding ids and types on meta
        // Keeps Previously existing ids if they can be matched
        void ImportAssetResources(const std::filesystem::path& asset, YAML::Node& meta);
        // Checks if all the asset resource files exist in library
        bool ValidateAssetResources(Resource::Type resource_type, const YAML::Node& meta) const;

    public:
        // Create a clean initial meta node
        YAML::Node CreateMeta();
        // Add asset hash value to any meta
        void UpdateAssetHash(const char* path, YAML::Node& meta) const;
        // With current meta state and resource index returns an existing eid of a new one if it doesnt exist
        UID ManageResourceUID(unsigned int resource_index, YAML::Node& meta);
    };
}
