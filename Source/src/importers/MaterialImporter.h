#pragma once

class aiMaterial;

namespace Hachiko
{
    class ResourceMaterial;
    class ResourceTexture;

    class MaterialImporter : public Importer
    {
        friend class ModelImporter;

    public:
        MaterialImporter();
        void Import(const char* path) override;
        void Save(const Resource* material) override;
        Resource* Load(const UID uid) override;
        Resource* Load(const char* material_path);
    private:
        void Import(const aiMaterial* ai_material, const UID& id = 0);
        ResourceTexture* ImportTexture(const aiMaterial* ai_material, aiTextureType type);
    };
} // namespace Hachiko