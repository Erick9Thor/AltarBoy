#pragma once

class aiMaterial;

namespace Hachiko
{
    class ResourceMaterial;

    class MaterialImporter : public Importer
    {
        friend class ModelImporter;

    public:
        MaterialImporter();
        void Import(const char* path) override;
        void Save(const Resource* material) override;
        Resource* Load(const UID uid) override;
        Resource* CherryImport(UID uid, const char* material_path);
    private:
        void Import(const aiMaterial* ai_material, const UID& id = 0);
    };
} // namespace Hachiko