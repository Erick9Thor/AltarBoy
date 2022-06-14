#pragma once

class aiScene;

namespace Hachiko
{
    class Importer;
    struct ResourceNode;

    class ModelImporter final : public Importer
    {
    public:
        ModelImporter() = default;
        ~ModelImporter() override = default;
        
        void Import(const char* path, YAML::Node& meta) override;
        void Save(UID id, const Resource* resource) override
        {
            assert(false && "Importer will never save, since model is only an asset and not a resource");
        };
        // This should not be called, since model is not a resource
        Resource* Load(UID id) override
        {
            assert(false && "Model importer will never load, since model is only an asset and not a resource");
            return nullptr;
        };

    private:
        void ImportModel(const char* path, const aiScene* scene, YAML::Node& meta);
        void ImportNode(GameObject* parent, const aiScene* scene, const aiNode* assimp_node, YAML::Node& meta, bool combine_intermediate_nodes);
    };
}