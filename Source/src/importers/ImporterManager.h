#pragma once

#include "importers/Importer.h"
#include "core/preferences/src/ResourcesPreferences.h"

namespace Hachiko
{
    class ImporterManager
    {
    public:
        ImporterManager();
        ~ImporterManager();
        void Import(const std::string& asset_path, Resource::Type asset_type);
        Resource* Load(Resource::Type type, UID resource_id);
        Resource* Load(Resource::Type type, const char* path);

    private:
        std::vector<std::pair<Importer::Type, Importer*>> importers;
        Importer* GetImporter(Resource::Type type);
        Importer::Type ToImporterType(Resource::Type type);
    };
}
