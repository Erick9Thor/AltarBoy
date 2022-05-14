#pragma once

#include "importers/Importer.h"

#include "ft2build.h"
#include "freetype.h"
#include "GLfont.h"

namespace Hachiko
{
    class FontImporter final : public Importer
    {
        friend class ModuleResources;

    public:
        FontImporter();
        ~FontImporter() override;

        void Import(const char* path, YAML::Node& meta) override;
        Resource* Load(UID id) override;
        void Save(UID id, const Resource* resource) override;

    private:
        FT_Library freetype_lib;
    };
} // namespace Hachiko
