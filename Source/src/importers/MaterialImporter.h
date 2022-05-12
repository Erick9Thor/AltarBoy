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

        void Import(const char* path, UID uid) override;
        void Save(const Resource* material) override;
        Resource* Load(UID id) override;
        void CreateMaterial(const std::string& name);
    
    private:
        void Import(aiMaterial* ai_material, const UID id);
        ResourceTexture* ImportTexture(const aiMaterial* ai_material, aiTextureType type);
    };
}