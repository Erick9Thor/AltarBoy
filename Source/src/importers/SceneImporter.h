#pragma once

#include "Importer.h"

namespace Hachiko
{
    class Scene;
    class ResourceScene;
    class SceneImporter : public Importer
    {
    public:
        SceneImporter() = default;
        ~SceneImporter() override = default;

        // Copies the scene file to lib
        void Import(const char* path, YAML::Node& meta) override;
        // Stores current scene resource state to assets and lib (since is an asset we edit in engine)
        void Save(UID id, const Resource* material) override;
        Resource* Load(UID id) override;
        // Create new scene asset
        UID CreateSceneAsset(Scene* scene);
        // Create new scene resource without loading from an asset
        ResourceScene* CreateSceneResource(Scene* scene);
    private:
    };
} // namespace Hachiko