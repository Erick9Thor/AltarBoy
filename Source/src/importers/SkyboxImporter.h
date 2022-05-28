#pragma once

#include "importers/Importer.h"

namespace Hachiko
{
    class SkyboxImporter final : public Importer
    {
        friend class ModuleResources;

    public:
        SkyboxImporter() = default;
        ~SkyboxImporter() override = default;

        void Import(const char* path, YAML::Node& meta) override;
        Resource* Load(UID id) override;

        void Save(UID id, const Resource* resource) override;

    private:
    };
} // namespace Hachiko
