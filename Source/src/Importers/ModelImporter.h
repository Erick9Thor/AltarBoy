#pragma once

class aiScene;

namespace Hachiko
{
    class Importer;

    class ModelImporter final : public Importer
    {
    public:
        ModelImporter();
        ~ModelImporter() override = default;
        void Import(const char* path) override;
        Resource* Load(const UID id) override;
        void Save(const Resource* resource) override;

    private:
        void ImportModel(const aiScene* scene, YAML::Node& ticket);
    };
}

