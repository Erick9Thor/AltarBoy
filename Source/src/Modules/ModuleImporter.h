#pragma once

#include "Module.h"
#include "importers/Importer.h"
#include "core/preferences/src/ResourcesPreferences.h"

namespace Hachiko
{
    class ModuleImporter : public Module
    {
    public:
        ModuleImporter();
        bool Init() override;
        bool CleanUp() override;

        bool RestoreLibrary();
        void ImportAsset(const std::string& asset_path, Hachiko::Resource::Type asset_type);

    private:
        std::vector<std::pair<Importer::Type, Importer*>> importers;
        Importer::Type ToImporterType(Resource::Type type);
    };
}
