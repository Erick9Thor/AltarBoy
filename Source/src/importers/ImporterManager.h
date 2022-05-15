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
        
        // If any id is defined it will be assigned to the resource, used to keep meta id but regenerate it
        void Import(const std::filesystem::path& asset_path, Resource::Type asset_type, UID id = 0);
        Resource* Load(Resource::Type type, UID id);
        bool IsImported(const char* path, Resource::Type type) const;

        void ImportWithMeta(const std::filesystem::path& asset_path, Resource::Type asset_type, YAML::Node& meta) const;
        void DeleteWithMeta(Resource::Type asset_type, const YAML::Node& meta) const;

        Importer* GetImporter(Resource::Type type) const;
        Importer::Type ToImporterType(Resource::Type type) const;
        // If any id is defined it will be assigned to the resource, used to keep meta id but regenerate it
        YAML::Node CreateMeta(const char* path, const Resource::Type resource_type, UID id = 0) const;
        void UpdateMeta(const char* path, YAML::Node& meta) const;
    };
}
