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

        void Import(const char* path, UID uid) override;
        Resource* Load(UID id) override;

        void Save(const Resource* resource) override;

    private:
        Resource* ImportFont(const char* path, UID uid);
        FT_Library freetype_lib;
    };
} // namespace Hachiko
