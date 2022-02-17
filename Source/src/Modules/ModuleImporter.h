#pragma once

#include "Module.h"
#include "importers/Importer.h"
#include "core/preferences/editor/ResourcesPreferences.h"

class ModuleImporter : public Module
{
public:
    ModuleImporter();
    bool Init() override;
    bool CleanUp() override;

    bool RestoreLibrary();
    void ImportAsset(const std::filesystem::path& asset_path, BoxerEngine::ResourceType asset_type);

private:
    BoxerEngine::ResourcesPreferences* preferences;
    std::vector<std::pair<BoxerEngine::Importer::Type, BoxerEngine::Importer*>> importers;

    BoxerEngine::Importer::Type ToImporterType(BoxerEngine::ResourceType type);
};
