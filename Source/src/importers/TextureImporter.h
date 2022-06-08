#pragma once

#include "importers/Importer.h"

namespace Hachiko
{
    class ResourceTexture;
    
    class TextureImporter final : public Importer
    {
        friend class MaterialImporter;
        friend class ModuleResources;
        friend class SkyboxImporter;

    public:
        TextureImporter() = default;
        ~TextureImporter() override = default;

        void Import(const char* path, YAML::Node& meta) override;
        Resource* Load(UID id) override;

        uint64_t GetExtraHash(const YAML::Node& meta);
        bool OutdatedExtraHash(const YAML::Node& meta);

        void Save(UID id, const Resource* resource) override;
        void SaveTextureAsset(const ResourceTexture* texture);
        ResourceTexture* CreateTextureAssetFromAssimp(const std::string& model_path, const aiMaterial* ai_material, aiTextureType type);
    private:
        
    };
}
