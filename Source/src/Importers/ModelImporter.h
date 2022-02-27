#pragma once

#include "../Resources/ResourceModel.h"

#include <assimp/scene.h>

namespace Hachiko
{
    class Importer;

    class ModelImporter final : public Importer
    {
    public:
        ModelImporter();
        ~ModelImporter() override = default;
        void Import(const char* path) override;
        void Load(UID id) override;
        void Save() override;

    private:
        void ImportModel(const aiScene* scene, YAML::Node& ticket);
    };
}

