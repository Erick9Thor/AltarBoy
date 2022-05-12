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

        // Copies the material file to lib
        void Import(const char* path, YAML::Node& meta) override;
        // Stores current material resource state to assets and lib
        void Save(const Resource* material) override;
        Resource* Load(UID id) override;
        // Create new material
        void CreateEmptyMaterial(const std::string& name);
        void CreateMaterialAssetFromAssimp(const std::string& model_path, aiMaterial* ai_material, UID uid);    
    private:
        
        
    };
}