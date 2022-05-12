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

        void Import(const char* path, YAML::Node& meta) override;
        void Save(const Resource* material) override;
        Resource* Load(UID id) override;
        void CreateMaterial(const std::string& name);
    
    private:
        void Import(aiMaterial* ai_material, YAML::Node& meta);
        ResourceTexture* ImportTexture(const aiMaterial* ai_material, aiTextureType type);
    };
}