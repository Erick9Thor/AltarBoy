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
        bool CheckIfImported(const char* path) override
        {
            return false;
        }
        
        Resource* Load(const char* material_path);
        void CreateMaterial(const std::string name);
    private:
        void Import(aiMaterial* ai_material, const UID& id = 0);
        ResourceTexture* ImportTexture(const aiMaterial* ai_material, aiTextureType type);
    };
} // namespace Hachiko