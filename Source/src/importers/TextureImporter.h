#pragma once

#include "importers/Importer.h"

namespace Hachiko
{

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

        void Save(UID id, const Resource* resource) override;
        ResourceTexture* CreateTextureAssetFromAssimp(const std::string& model_path, const aiMaterial* ai_material, aiTextureType type);
    private:
    };
}
