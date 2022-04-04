#pragma once

#include "importers/Importer.h"
#include "core/preferences/src/ResourcesPreferences.h"

namespace Hachiko
{
    class ImporterManager
    {
        friend class ModuleResources;
    public:
        ImporterManager();
        ~ImporterManager();
        void Import(Resource::Type type, const std::string& path);
        Resource* Load(Resource::Type type, const char* path);
        bool CheckIfImported(Resource::Type type, const char* path) const;
       
    private:
        std::vector<std::pair<Importer::Type, Importer*>> importers;
        Importer* GetImporter(Resource::Type type) const;
        Importer::Type ToImporterType(Resource::Type type) const;

    };
}
