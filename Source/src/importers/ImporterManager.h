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

        std::map<Resource::Type, Importer*> importers;
        
        // If any id is defined it will be assigned to the resource, used to keep meta id but regenerate it
        void Import(const std::filesystem::path& asset_path, Resource::Type asset_type, YAML::Node& meta);
        Resource* Load(Resource::Type type, UID id);
        void Delete(UID uid, Resource::Type resource_type) const;

        Importer* GetImporter(Resource::Type type) const;
    };
} // namespace Hachiko
