#pragma once

#include "Importer.h"

namespace Hachiko
{
    class ResourcePrefab;

    class PrefabImporter : public Importer
    {
    public:
        PrefabImporter() = default;
        void Import(const char* path, YAML::Node& meta) override;
        void Save(UID id, const Resource* mesh) override;
        Resource* Load(UID id) override;
    };
} // namespace Hachiko
