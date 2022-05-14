#pragma once

class aiScene;

namespace Hachiko
{
    class Importer;
    struct ResourceNode;

    class ModelImporter final : public Importer
    {
    public:
        ModelImporter();
        ~ModelImporter() override = default;
        
        void Import(const char* path, YAML::Node& meta) override;
        void Save(UID id, const Resource* resource) override;
        // This should not be called, since model is not a resource
        Resource* Load(UID id) override;

    private:
        void ImportModel(const char* path, const aiScene* scene, YAML::Node& meta);
        void ImportNode(GameObject* parent, const aiScene* scene, const aiNode* assimp_node, YAML::Node& meta, bool load_auxiliar);
        void LoadChildren(YAML::Node& node, YAML::Node& meshes, YAML::Node& materials, std::vector<ResourceNode*>& children);
    };
}