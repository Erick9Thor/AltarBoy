#pragma once

#include "importers/Importer.h"

namespace Hachiko
{
    class ImporterManager
    {
        friend class ModuleResources;
    public:
        ~ImporterManager();

    private:
        ImporterManager();

        std::map<Importer::Type, Importer*> importers;
        
        void Import(const std::filesystem::path& asset_path, Resource::Type asset_type);
        Resource* Load(Resource::Type type, UID id);
        bool IsImported(const char* path, Resource::Type type) const;

        void ImportWithMeta(const std::filesystem::path& asset_path, Resource::Type asset_type, YAML::Node& meta) const;
        void DeleteWithMeta(Resource::Type asset_type, const YAML::Node& meta) const;

        Importer* GetImporter(Resource::Type type) const;
        Importer::Type ToImporterType(Resource::Type type) const;
        YAML::Node CreateMeta(const char* path, const Resource::Type resource_type) const;
        void UpdateMeta(const char* path, const Resource::Type resource_type, YAML::Node& meta) const;
    };
}
