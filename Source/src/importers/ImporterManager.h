#pragma once

#include "importers/Importer.h"

namespace Hachiko
{
    class ImporterManager
    {
        friend class ModuleResources;
    public:
        ImporterManager();
        ~ImporterManager();
        void Import(const std::filesystem::path& asset_path, Resource::Type asset_type);
        Resource* Load(Resource::Type type, const char* path);
        bool IsImported(const char* path, Resource::Type type) const;
       
    private:
        std::vector<std::pair<Importer::Type, Importer*>> importers;
        Importer* GetImporter(Resource::Type type) const;
        Importer::Type ToImporterType(Resource::Type type) const;
        YAML::Node CreateMeta(const char* path);
    };
}
