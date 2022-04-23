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
        Resource* Load(Resource::Type type, const char* path); // TODO: To remove
        Resource* Load(Resource::Type type, UID id);
        bool IsImported(const char* path, Resource::Type type) const;


        Importer* GetImporter(Resource::Type type) const;
        Importer::Type ToImporterType(Resource::Type type) const;
        YAML::Node CreateMeta(const char* path, const Resource::Type resource_type);
    };
}
