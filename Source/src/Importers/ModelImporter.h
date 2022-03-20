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
        void Import(const char* path) override;
        Resource* Load(const UID id) override;
        Resource* Load(const char* model_path);
        void Save(const Resource* resource) override;

    private:
        void ImportModel(const aiScene* scene, YAML::Node& ticket);
        void ImportNode(const aiNode* assimp_node, YAML::Node& node);
        void LoadChilds(YAML::Node& node, std::vector<ResourceNode*>& childs);
    };
}