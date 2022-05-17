#pragma once

#include "importers/Importer.h"

namespace Hachiko
{
    class MaterialImporter;
    class TextureImporter;
    class FontImporter;
    class PrefabImporter;
    class ModelImporter;
    class AnimationImporter;
    class MeshImporter;

    class ImporterManager
    {
        friend class ModuleResources;
    public:
        ~ImporterManager();

    private:
        ImporterManager();

        std::map<Resource::AssetType, Importer*> asset_importers;
        std::map<Resource::Type, Importer*> resource_importers;
        
        // If any id is defined it will be assigned to the resource, used to keep meta id but regenerate it
        std::vector<UID> ImportAsset(const std::filesystem::path& asset_path, Resource::AssetType asset_type, YAML::Node& meta);
        Resource* LoadResource(Resource::Type type, UID id);
        void DeleteResource(UID uid, Resource::Type resource_type) const;

        Importer* GetAssetImporter(Resource::AssetType type) const;
        Importer* GetResourceImporter(Resource::Type type) const;


        // They exist both as assets and as resources
        MaterialImporter* material = nullptr;
        TextureImporter* texture = nullptr;
        FontImporter* font = nullptr;
        PrefabImporter* prefab = nullptr;

        // It doesnt have its own resource type (we use prefabs)
        ModelImporter* model = nullptr;

        // They dont have their own asset type
        AnimationImporter* animation = nullptr;
        MeshImporter* mesh = nullptr;
    };
} // namespace Hachiko
